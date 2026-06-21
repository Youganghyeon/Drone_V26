/*
 * calibration.c
 *
 *  Created on: 2026. 6. 10.
 *      Author: yougang
 */


#if 0
#include "calibration.h"

void Calibration_sensor(void)
{
  ICM20602_Writebyte(0x13, ((gyro_x_offset*-2)>>8));
  ICM20602_Writebyte(0x14, (gyro_x_offset*-2));

  ICM20602_Writebyte(0x15, ((gyro_y_offset*-2)>>8));
  ICM20602_Writebyte(0x16, (gyro_y_offset*-2));

  ICM20602_Writebyte(0x17, ((gyro_z_offset*-2)>>8));
  ICM20602_Writebyte(0x18, (gyro_z_offset*-2));
}





  if(EP_PIDGain_Read(0, &roll.in.kp, &roll.in.ki, &roll.in.kd) != 0 ||
      EP_PIDGain_Read(1, &roll.out.kp, &roll.out.ki, &roll.out.kd) != 0 ||
      EP_PIDGain_Read(2, &pitch.in.kp, &pitch.in.ki, &pitch.in.kd) != 0 ||
      EP_PIDGain_Read(3, &pitch.out.kp, &pitch.out.ki, &pitch.out.kd)!=0 ||
      EP_PIDGain_Read(4, &yaw_heading.kp, &yaw_heading.ki, &yaw_heading.kd) !=0 ||
      EP_PIDGain_Read(5, &yaw_rate.kp, &yaw_rate.ki, &yaw_rate.kd) != 0)
  {
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);

     TIM3->PSC = 2000;
     HAL_Delay(100);
     TIM3->PSC = 1500;
     HAL_Delay(100);
     TIM3->PSC = 1000;
     HAL_Delay(100);

     LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

     HAL_Delay(500);
     printf("\nCouln't read PID gain.\n");
  }
  else
  {
    Encode_Msg_PID_Gain(&telemetry_tx_buf[0],0,roll.in.kp, roll.in.ki, roll.in.kd);
    Encode_Msg_PID_Gain(&telemetry_tx_buf[0],1,roll.out.kp, roll.out.ki, roll.out.kd);
    Encode_Msg_PID_Gain(&telemetry_tx_buf[0],2,pitch.in.kp, pitch.in.ki, pitch.in.kd);
    Encode_Msg_PID_Gain(&telemetry_tx_buf[0],3,pitch.out.kp, pitch.out.ki, pitch.out.kd);
    Encode_Msg_PID_Gain(&telemetry_tx_buf[0],4,yaw_heading.kp, yaw_heading.ki, yaw_heading.kd);
    Encode_Msg_PID_Gain(&telemetry_tx_buf[0],5,yaw_rate.kp, yaw_rate.ki, yaw_rate.kd);
  }


  while(Is_Ibus_Received() == 0) // FS-i6ab Data recevied?
  {
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
    TIM3->PSC = 2000;
    HAL_Delay(200);
    LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
    HAL_Delay(200);
  };
  if(iBus.SwC == 2000)
  {

    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);

    TIM3->PSC = 1500;
    HAL_Delay(100);
    TIM3->PSC = 2000;
    HAL_Delay(100);
    TIM3->PSC = 1500;
    HAL_Delay(100);
    TIM3->PSC = 2000;
    HAL_Delay(100);
    LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

    ESC_callibration();
    while(iBus.SwC != 1000)
    {
      Is_Ibus_Received();
      LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);

      TIM3->PSC = 1500;
      HAL_Delay(100);
      TIM3->PSC = 2000;
      HAL_Delay(100);
      LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

    }
  }
  else if(iBus.SwC == 1500)
  {
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
    TIM3->PSC = 1500;
    HAL_Delay(100);
    TIM3->PSC = 2000;
    HAL_Delay(100);
    TIM3->PSC = 1500;
    HAL_Delay(100);
    TIM3->PSC = 2000;
    HAL_Delay(100);
    LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

    BNO080_Calibration();
    while(iBus.SwC != 1000)
    {
      Is_Ibus_Received();
      LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);

      TIM3->PSC = 1500;
      HAL_Delay(100);
      TIM3->PSC = 2000;
      HAL_Delay(100);
      LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

    }
  }
  while(Is_iBus_Throttle_Min() == 0 || iBus.SwA == 2000)
  {
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
    TIM3->PSC = 1000;
    HAL_Delay(70);
    LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
    HAL_Delay(70);
  };


  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);

  TIM3->PSC = 2000;
  HAL_Delay(100);
  TIM3->PSC = 1500;
  HAL_Delay(100);
  TIM3->PSC = 1000;
  HAL_Delay(100);

  LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

