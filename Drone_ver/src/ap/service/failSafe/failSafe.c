/*
 * failSafe.c
 *
 *  Created on: 2026. 6. 10.
 *      Author: yougang
 */
#include "failSafe.h"
#include "led.h"
#include "buzzer.h"

#define  ishwCan          (isledInit() && isbuzzerInit())

bool  Alarm_SensorError(uint8_t sensor_ch)
{
  bool ret= false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 1000);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 1500);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(100);
    switch(sensor_ch)
    {
      case DEF_SENSOR1:
        ledToggle(DEF_LED_1);
        break;
      case DEF_SENSOR2:
        ledToggle(DEF_LED_2);
        break;
      case DEF_SENSOR3:
        ledToggle(DEF_LED_3);
        break;
    }
    ret = true;
  }
  return ret;
}

bool Alarm_RCLost(void)
{
  bool ret = false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 1500);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 1000);
    delay(100);
    ret= true;
  }
  return ret;
}

bool Alarm_LowBattery(void)
{
  bool ret = false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(200);
    ret=true;
  }
  return ret;
}

bool Alarm_FailSafe(uint8_t flag)
{
  if(ishwCan)
  {

  }
}

bool Alarm_EscCalibration(void)
{

}


bool Alarm_Fail_PidRead(void)
{

}
