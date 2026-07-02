/*
 * flight.c
 *
 *  Created on: 2026. 6. 10.
 *      Author: yougang
 */

#include "flight.h"

static void All_MotorOutput(void);
static void All_Motor_Stop(void);
static void Calculate_CCR(Double_PID_tbl* pid_pitch, Double_PID_tbl* pid_roll, float yaw_out);

static DroneState   state       = STATE_DISARMED;
static uint8_t      swA_prev    = 0;
static uint32_t     ccr1, ccr2, ccr3, ccr4;
static bool         isInit      = false;
static float        yaw_heading_reference = 0.0f;  // 내부 관리

static DroneTm_tbl* p_droneTm  = NULL;
static Sensor_tbl*  p_sensor   = NULL;

/* ────────────────────────────────────────
 * 초기화
 * ──────────────────────────────────────── */
bool Drone_Service_Init(void)
{
  bool s_ret = false;
  bool d_ret = false;

  state = STATE_DISARMED;
  All_Motor_Stop();


  if(isSensorInit() == true)
  {
    p_sensor  = sensorGetData();
    s_ret     = true;
  }

  if(IsdroneTmInit() == true)
  {
    p_droneTm = droneLinkData();
    d_ret     = true;
  }

  if(s_ret && d_ret)
  {
    isInit = true;
  }

  return isInit;
}

/* ────────────────────────────────────────
 * 상태 반환
 * ──────────────────────────────────────── */
DroneState Drone_Get_State(void)
{
  return state;
}

/* ────────────────────────────────────────
 * 아밍 업데이트 (메인루프 호출)
 * ──────────────────────────────────────── */
void Drone_Arming_Update( Double_PID_tbl* pid_roll,Double_PID_tbl* pid_pitch,
                         Single_PID_tbl* pid_yawHeading, Single_PID_tbl* pid_yawRate)
{
  if(p_droneTm->switch_ch[DEF_SwA] != Switch_low)
  {
    if(state != STATE_DISARMED)
    {
      state = STATE_DISARMED;
      Reset_All_PID_Integrator(pid_roll,pid_pitch,pid_yawHeading,pid_yawRate);
      All_Motor_Stop();
    }
    swA_prev = p_droneTm->switch_ch[DEF_SwA];
    return;
  }

  /* SwA 상승 엣지 감지 → 아밍 시도 */
  if(p_droneTm->switch_ch[DEF_SwA] == Switch_low && swA_prev != Switch_low)
  {
    if(p_droneTm->setthrottle < THROTTLE_ARM_MAX)
    {
      state                     = STATE_ARMED_IDLE;
      yaw_heading_reference     = p_sensor->yaw;         // 내부 저장
      pid_yawHeading->reference = p_sensor->yaw;     // ap단 PID에도 반영
    }
    else
    {
      state = STATE_ARMING_WARN;
    }
  }

  /* Failsafe */
  if(p_droneTm->failsafe_status)
  {
    state = STATE_FAILSAFE;
    Reset_All_PID_Integrator(pid_roll,pid_pitch,pid_yawHeading,pid_yawRate);
    All_Motor_Stop();
  }

  swA_prev = p_droneTm->switch_ch[DEF_SwA];
}

/* ────────────────────────────────────────
 * FSM 1ms 업데이트 (1ms 타이머 호출)
 * ──────────────────────────────────────── */
void Drone_FSM_1ms_Update( Double_PID_tbl* pid_roll,Double_PID_tbl* pid_pitch,
                           Single_PID_tbl* pid_yawHeading, Single_PID_tbl* pid_yawRate)
{
  switch(state)
  {
    case STATE_DISARMED:
    case STATE_FAILSAFE:
      Reset_All_PID_Integrator(pid_roll,pid_pitch,pid_yawHeading,pid_yawRate);
    case STATE_ARMING_WARN:
      All_Motor_Stop();
      break;
    case STATE_ARMED_IDLE:
      Reset_All_PID_Integrator(pid_roll,pid_pitch,pid_yawHeading,pid_yawRate);
      yaw_heading_reference = p_sensor->yaw;
      escOutput(DEF_ESC1, MOTOR_IDLE_MIN);
      escOutput(DEF_ESC2, MOTOR_IDLE_MIN);
      escOutput(DEF_ESC3, MOTOR_IDLE_MIN);
      escOutput(DEF_ESC4, MOTOR_IDLE_MIN);
      if(p_droneTm->setthrottle > THROTTLE_MIN)
      {
        state = STATE_ARMED_FLIGHT;;
      }
      break;

    case STATE_ARMED_FLIGHT:
      if(p_droneTm->setthrottle < THROTTLE_MIN)
      {
        state = STATE_ARMED_IDLE;
        break;
      }

      Double_PID_Calc(pid_pitch, (p_droneTm->setPitch - 1500) * 0.1f,
                      p_sensor->pitch, p_sensor->gyro_x);
      Double_PID_Calc(pid_roll,  (p_droneTm->setRoll  - 1500) * 0.1f,
                      p_sensor->roll,  p_sensor->gyro_y);

      if(p_droneTm->setyaw < YAW_DEADZONE_LOW ||p_droneTm->setyaw > YAW_DEADZONE_HIGH)
      {
        yaw_heading_reference = p_sensor->yaw;
        Single_PID_Yaw_Rate_Calc(pid_yawRate, (float)(p_droneTm->setyaw - 1500), p_sensor->gyro_z);
        Calculate_CCR(pid_pitch, pid_roll, pid_yawRate->pid_result);
      }
      else
      {
        Single_PID_Yaw_Heading_Calc(pid_yawHeading, yaw_heading_reference,p_sensor->yaw, p_sensor->gyro_z);
        Calculate_CCR(pid_pitch, pid_roll, pid_yawHeading->pid_result);
      }
      All_MotorOutput();
      break;

    default:
      state  = STATE_DISARMED;
      break;
  }
}

/* ────────────────────────────────────────
 * 내부 함수
 * ──────────────────────────────────────── */
static void Calculate_CCR(Double_PID_tbl* pid_pitch, Double_PID_tbl* pid_roll, float yaw_out)
{
  uint32_t base = MOTOR_STOP + 500 + (p_droneTm->setthrottle - 1000) * 10;

  ccr1 = base - pid_pitch->inner.pid_result + pid_roll->inner.pid_result - yaw_out;
  ccr2 = base + pid_pitch->inner.pid_result + pid_roll->inner.pid_result + yaw_out;
  ccr3 = base + pid_pitch->inner.pid_result - pid_roll->inner.pid_result - yaw_out;
  ccr4 = base - pid_pitch->inner.pid_result - pid_roll->inner.pid_result + yaw_out;
}

static void All_MotorOutput(void)
{
  #define CLAMP(v) ((v) > MOTOR_MAX ? MOTOR_MAX : (v) < MOTOR_STOP ? MOTOR_STOP : (v))
  escOutput(DEF_ESC1, CLAMP(ccr1));
  escOutput(DEF_ESC2, CLAMP(ccr2));
  escOutput(DEF_ESC3, CLAMP(ccr3));
  escOutput(DEF_ESC4, CLAMP(ccr4));
}

static void All_Motor_Stop(void)
{
  escOutput(DEF_ESC1, MOTOR_STOP);
  escOutput(DEF_ESC2, MOTOR_STOP);
  escOutput(DEF_ESC3, MOTOR_STOP);
  escOutput(DEF_ESC4, MOTOR_STOP);
}


