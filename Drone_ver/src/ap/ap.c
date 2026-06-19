/*
 * ap.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "dronetm.h"
#include "ap.h"#include "gsc_tm.h"
#include "sensor.h"
#include "esc.h"
#include "service/gscMsg/gscMsg.h"
#include "pid.h"

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
  (void)file;
  uartWrite(DEF_UART6, (uint8_t*)ptr, (uint32_t)len);
  return len;
}

Double_PID_tbl roll;
Double_PID_tbl pitch;
Single_PID_tbl yaw_heading;
Single_PID_tbl yaw_rate;

static DroneTm_tbl* droneTm;
static Sensor_tbl* sensor_data;
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
      sensorCalibration(1);
    }
    sensorCalibration(2);
  }

}

static float    BatVolt=0.0;
static uint16_t adcVolt=0;

void apMain(void)
{
  uint32_t premillis=0;
  adcReceive_DMA(DEF_ADC1, (uint32_t*)&adcVolt, 1);
  while(1)
  {
    if(millis()-premillis>=500)
    {
      ledToggle(DEF_LED_2);
      ledToggle(DEF_LED_3);
      premillis=millis();
    }
    sensorUpdate();
    droneTmUpdate();

    if(droneTm->is_Received == true && (!droneTm->failsafe_status))
    {
      uint32_t output = (uint32_t)(10500  + (droneTm->setthrottle - 1000) * 10.5);
      escOutput(DEF_ESC1, output);
      escOutput(DEF_ESC2, output);
      escOutput(DEF_ESC3, output);
      escOutput(DEF_ESC4, output);
    }
    if(Is1msFlag(DEF_TIM7))
    {
      clear1msFlag(DEF_TIM7);
    }

    if(Is20msFlag(DEF_TIM7))
    {
      EncodeMsg_AHRS();
      clear20msFlag(DEF_TIM7);
    }
    //cliMain();
  }

//
//    LPS22HH_GetInfo(&LPS22HH,LPS22HH_GetPress);
//    LPS22HH_GetAlt(&LPS22HH, TEMP_CORRECT);
//    LPS22HH_GetAltFilt(&LPS22HH);
//
//
//    ICM20602_GetInfo(&ICM20602, AxisGyroRaw);
//    BNO080_ReadInfo(&BNO080);
//
//    M8N_ReceivePacket(&M8N);
//    if(M8N.RxBuf.m8n_cplt_flag== 1)
//    {
//      M8N.RxBuf.m8n_cplt_flag = 0;
//
//      if(M8N_Checksum_Check(&M8N.RxBuf.buf[0], 36) == 1)
//      {
//        ledToggle(DEF_LED_1);
//        M8N_Parsing(&M8N.RxBuf.buf[0], &M8N.posllh);
//
//        //  printf("LAT: %d\tLON: %d\t Height: %d\n", M8N.posllh.lat, M8N.posllh.lon, M8N.posllh.height);
//      }
//    }
//
//    FSIA6B_RecivePacket(&IA6B);
//    if(IA6B.ibus_rx_cplt_flag == 1)
//    {
//      IA6B.ibus_rx_cplt_flag = 0;
//      if(FSIA6B_Check_checkSum(&IA6B, 32) == 1)
//      {
//        FSIA6B_Parsing(&IA6B);
//        if(FSIA6B_isFailsafe(&IA6B) == 1)
//        {
//
//        }
//        else
//        {
//
//        }
//      }
//    }
//
//    if(Is20msFlag(DEF_TIM7)==1)
//    {
//
//      //ROHS_Read();
//      EncodeMsg_AHRS(&ROHS, &BNO080, &LPS22HH, &IA6B);
//      ROHS_Write(&ROHS,&ROHS.txBuf[0],20);
//      clear20msFlag(DEF_TIM7);
//    }
//
//
//    BatVolt = adcVolt * 0.003619f;
//  }
}
