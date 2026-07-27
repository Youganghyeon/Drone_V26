/*
 * flight_interface.c
 *
 *  Created on: 2026. 7. 4.
 *      Author: yougang
 */

/*
 * flight.c
 */
#include "flight_if.h"

static void All_MotorOutput(void);
static void All_Motor_Stop(void);
static void Calculate_CCR(uint32_t base, Double_PID_tbl* pid_pitch, Double_PID_tbl* pid_roll, float yaw_out);

extern const FlightMode_If_t FlightMode_Normal;
extern const FlightMode_If_t FlightMode_Alt;
extern const FlightMode_If_t FlightMode_Gps;

static const FlightMode_If_t* mode_table[MODE_MAX_NUM] = {
    [Normal_Mode] = &FlightMode_Normal,
    //[Alt_Mode]    = &FlightMode_Alt,
    //[Gps_Mode]    = &FlightMode_Gps,
};

static const FlightMode_If_t* current_mode = &FlightMode_Normal;
static FlightPidCtx_t pid_ctx;

static DroneState   state       = STATE_DISARMED;
static uint8_t      swA_prev    = 0;
static uint32_t     ccr1, ccr2, ccr3, ccr4;
static bool         isInit      = false;
static float        yaw_heading_reference = 0.0f;
static uint32_t     throttle_base = 0;
static DroneTm_tbl* p_droneTm  = NULL;
static Sensor_tbl*  p_sensor   = NULL;

/* ────────────────────────────────────────
 * 초기화
 * ──────────────────────────────────────── */
bool Drone_Service_Init(Double_PID_tbl* pid_roll, Double_PID_tbl* pid_pitch,
                        Single_PID_tbl* pid_yawHeading, Single_PID_tbl* pid_yawRate,
                        Double_PID_tbl* pid_alt)
{
    bool s_ret = false;
    bool d_ret = false;

    state = STATE_DISARMED;
    All_Motor_Stop();

    /* ctx에 PID 포인터들을 최초 1회 등록 */
    pid_ctx.pid_roll        = pid_roll;
    pid_ctx.pid_pitch       = pid_pitch;
    pid_ctx.pid_yawHeading  = pid_yawHeading;
    pid_ctx.pid_yawRate     = pid_yawRate;
    pid_ctx.pid_alt         = pid_alt;

    if(isSensorInit() == true)
    {
        p_sensor = sensorGetData();
        s_ret    = true;
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
 * 모드 선택 (SwC 토글 카운트 기반)
 * ──────────────────────────────────────── */
void Drone_Mode_Select(Control_Mode_t mode)
{
    current_mode = mode_table[mode];

    if(current_mode->entry)
    {
        current_mode->entry(&pid_ctx);
    }
}

/* ────────────────────────────────────────
 * 아밍 업데이트 (메인루프 호출)
 * ──────────────────────────────────────── */
void Drone_Arming_Update(void)
{
    if(p_droneTm->switch_ch[DEF_SwA] != Switch_low)
    {
        if(state != STATE_DISARMED)
        {

            state = STATE_DISARMED;
            current_mode->reset_pid(&pid_ctx);
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
            state                          = STATE_ARMED_IDLE;
            yaw_heading_reference          = p_sensor->yaw;
            pid_ctx.pid_yawHeading->reference = p_sensor->yaw;
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
        current_mode->reset_pid(&pid_ctx);
        All_Motor_Stop();
    }

    swA_prev = p_droneTm->switch_ch[DEF_SwA];
}

/* ────────────────────────────────────────
 * FSM 1ms 업데이트 (1ms 타이머 호출)
 * ──────────────────────────────────────── */
void Drone_FSM_1ms_Update(void)
{
    switch(state)
    {
        case STATE_DISARMED:
        case STATE_FAILSAFE:
            current_mode->reset_pid(&pid_ctx);
        case STATE_ARMING_WARN:
            All_Motor_Stop();
            break;

        case STATE_ARMED_IDLE:
            current_mode->reset_pid(&pid_ctx);
            yaw_heading_reference = p_sensor->yaw;
            escOutput(DEF_ESC1, MOTOR_IDLE_MIN);
            escOutput(DEF_ESC2, MOTOR_IDLE_MIN);
            escOutput(DEF_ESC3, MOTOR_IDLE_MIN);
            escOutput(DEF_ESC4, MOTOR_IDLE_MIN);
            if(p_droneTm->setthrottle > THROTTLE_MIN)
            {
                state = STATE_ARMED_FLIGHT;
            }
            break;

        case STATE_ARMED_FLIGHT:
            if(p_droneTm->setthrottle < THROTTLE_MIN)
            {
                state = STATE_ARMED_IDLE;
                break;
            }

            Double_PID_Calc(pid_ctx.pid_pitch, (p_droneTm->setPitch - 1500) * 0.1f,
                            p_sensor->pitch, p_sensor->gyro_x);
            Double_PID_Calc(pid_ctx.pid_roll, (p_droneTm->setRoll - 1500) * 0.1f,
                            p_sensor->roll, p_sensor->gyro_y);

            if(p_droneTm->setyaw < YAW_DEADZONE_LOW || p_droneTm->setyaw > YAW_DEADZONE_HIGH)
            {
                yaw_heading_reference = p_sensor->yaw;
                Single_PID_Yaw_Rate_Calc(pid_ctx.pid_yawRate,
                                         (float)(p_droneTm->setyaw - 1500), p_sensor->gyro_z);
                throttle_base = current_mode->calc_throttle_base(p_droneTm, &pid_ctx);
                Calculate_CCR(throttle_base, pid_ctx.pid_pitch, pid_ctx.pid_roll,
                             pid_ctx.pid_yawRate->pid_result);
            }
            else
            {
                Single_PID_Yaw_Heading_Calc(pid_ctx.pid_yawHeading, yaw_heading_reference,
                                            p_sensor->yaw, p_sensor->gyro_z);
                throttle_base = current_mode->calc_throttle_base(p_droneTm, &pid_ctx);
                Calculate_CCR(throttle_base, pid_ctx.pid_pitch, pid_ctx.pid_roll,
                             pid_ctx.pid_yawHeading->pid_result);
            }

            if(current_mode->update_1ms)
            {
                current_mode->update_1ms(&pid_ctx);
            }

            All_MotorOutput();
            break;

        default:
            state = STATE_DISARMED;
            break;
    }
}

/* ────────────────────────────────────────
 * 20ms 업데이트
 * ──────────────────────────────────────── */
void Drone_20ms_Update(void)
{
    if(current_mode->update_20ms)
    {
        current_mode->update_20ms(&pid_ctx);
    }
}

/* ────────────────────────────────────────
 * 내부 함수
 * ──────────────────────────────────────── */
static void Calculate_CCR(uint32_t base, Double_PID_tbl* pid_pitch, Double_PID_tbl* pid_roll, float yaw_out)
{
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
