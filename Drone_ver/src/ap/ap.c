/*
 * ap.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "ap.h"
#include "module.h"
#include "service/gscMsg/gscMsg.h"
#include "service/flight//flight.h"
#include "pid.h"

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
  (void)file;
  uartWrite(DEF_UART6, (uint8_t*)ptr, (uint32_t)len);
  return len;
}

static Double_PID_tbl roll;
static Double_PID_tbl pitch;
static Single_PID_tbl yaw_heading;
static Single_PID_tbl yaw_rate;

static DroneTm_tbl* droneTm;
static Sensor_tbl*  sensor_data;

void apInit(void)
{
  /*------------------UART------------------*/
  uartOpen(DEF_UART1, 115200);  // ROHS
  uartOpen(DEF_UART5, 115200);  //FSi6AB uart
  uartOpen(DEF_UART6, 9600);    //PC uart
  // cliOpen(DEF_UART6, 9600);
  uartOpen(DEF_UART4, 115200);

  /*------------------TIM------------------*/
  timOpen (DEF_TIM7);
  timOpen (DEF_TIM5_CH1);
  timOpen (DEF_TIM5_CH2);
  timOpen (DEF_TIM5_CH3);
  timOpen (DEF_TIM5_CH4);

  /*------------------ADC------------------*/
  adcOpen (DEF_ADC1);

  /*------------------SPI------------------*/

  spiOpen(DEF_HW_ICM20602);
  spiOpen(DEF_HW_LPS22HH);
  spiOpen(DEF_HW_BNO080);

  /*------------------I2C------------------*/
  i2cOpen(HW_DEF_I2C1);

  //  buzSetPitch(2000);
  //  delay(1000);
  //  buzSetPitch(1000);
  //  delay(1000);
  //buzDeinit();

  /*------------------Module------------------*/
  sensorInit();
  // sensorCalibration(ICM20602_Cali);

  gcsTmInit();

  droneTmInit();
  droneTm = droneLinkData();

  escInit();
  escOpen(DEF_ESC1);
  escOpen(DEF_ESC2);
  escOpen(DEF_ESC3);
  escOpen(DEF_ESC4);

  AT24C08_Open();

  /*------------------Service------------------*/
  ServiceMsg_Init();

  /*------------------Calibration------------------*/

  /* 1. Read PID -> GCS
   * 2. Ready until droneTm is connect
   * 3. According to SwC, do a calibration_task
   *  3-1. if SwC is High, escCalibration
   *    3-1-(1). Ready until SwC is Low
   *  3-2. if SwC is Idle(mid), bno080Calibration
   * 4.
   */


  /*------------------1. Read PID -> GCS------------------*/
  if(EP_PIDGain_Read(PID_Roll_in, &roll.inner.kp, &roll.inner.ki, &roll.inner.kd)       &&
      EP_PIDGain_Read(PID_Roll_out, &roll.outer.kp, &roll.outer.ki, &roll.outer.kd)     &&
      EP_PIDGain_Read(PID_Pitch_in, &pitch.inner.kp, &pitch.inner.ki, &pitch.inner.kd)  &&
      EP_PIDGain_Read(PID_Pitch_out, &pitch.outer.kp, &pitch.outer.ki, &pitch.outer.kd) &&
      EP_PIDGain_Read(PID_Yaw_in, &yaw_heading.kp, &yaw_heading.ki, &yaw_heading.kd)    &&
      EP_PIDGain_Read(PID_Yaw_out, &yaw_rate.kp, &yaw_rate.ki, &yaw_rate.kd) != 0)
  {
    MsgEncode_PID_Gain(PID_Roll_in, roll.inner.kp, roll.inner.ki, roll.inner.kd);
    MsgEncode_PID_Gain(PID_Roll_out, roll.outer.kp, roll.outer.ki, roll.outer.kd);
    MsgEncode_PID_Gain(PID_Pitch_in, pitch.inner.kp, pitch.inner.ki, pitch.inner.kd);
    MsgEncode_PID_Gain(PID_Pitch_out, pitch.outer.kp, pitch.outer.ki, pitch.outer.kd);
    MsgEncode_PID_Gain(PID_Yaw_in, yaw_heading.kp, yaw_heading.ki, yaw_heading.kd);
    MsgEncode_PID_Gain(PID_Yaw_out, yaw_rate.kp, yaw_rate.ki, yaw_rate.kd);
  }
  else
  {

  }

  /*------------------2. Ready until droneTm is connect------------------*/
  while(droneTm->is_Received != true)
  {
    droneTmUpdate();
  }

  /*------------------According to SwC, do a calibration_task------------------*/
  if(droneTm->switch_ch[DEF_SwC] == Switch_high)
  {
    escCalibration();
    while(droneTm->switch_ch[DEF_SwC] == Switch_low)
    {
    }
  }
  else if(droneTm->switch_ch[DEF_SwC] == Switch_IDLE)
  {
    while(droneTm->switch_ch[DEF_SwC] == Switch_IDLE)
    {
      sensorCalibration(BNO080_Cali_Step1);
      droneTmUpdate();
    }
    sensorCalibration(BNO080_Cali_Step2);
  }
  sensorCalibration(ICM20602_Cali);
}

static float    BatVolt=0.0;
static uint16_t adcVolt=0;

void apMain(void)
{
  uint32_t premillis=0;
  adcReceive_DMA(DEF_ADC1, (uint32_t*)&adcVolt, 1);
  while(1)
  {
    sensorUpdate();
    droneTmUpdate();
    gcsTmUpdate(gcsTmRxcplt_Func1); // Use Callback.
    Drone_Arming_Update(&roll, &pitch, &yaw_heading, &yaw_rate);

    if(millis()-premillis>=500)
    {
      ledToggle(DEF_LED_2);
      ledToggle(DEF_LED_3);
      premillis=millis();
    }

    if(Is1msFlag(DEF_TIM7))
    {
      Drone_FSM_1ms_Update(&roll, &pitch, &yaw_heading, &yaw_rate);
      clear1msFlag(DEF_TIM7);
    }

    if(Is20msFlag(DEF_TIM7))
    {
      EncodeMsg_AHRS();
      clear20msFlag(DEF_TIM7);
    }
    //cliMain();
    //    BatVolt = adcVolt * 0.003619f;
  }
}


