/*
 * failSafe.c
 *
 *  Created on: 2026. 6. 10.
 *      Author: yougang
 */
#include "failSafe.h"
#include "led.h"
#include "buzzer.h"
#include "module.h"

#define ishwCan (isledInit() && isBuzInit())

/* ────────────────────────────────────────
 * 센서 초기화 실패 알람
 * 오름차순 부저 + 해당 LED 토글
 * ──────────────────────────────────────── */
bool Alarm_SensorError(uint8_t sensor_ch)
{
  bool ret = false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 1000);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 1500);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 0);

    switch(sensor_ch)
    {
      case DEF_SENSOR1: ledToggle(DEF_LED_1); break;
      case DEF_SENSOR2: ledToggle(DEF_LED_2); break;
      case DEF_SENSOR3: ledToggle(DEF_LED_3); break;
      default: break;
    }
    ret = true;
  }
  return ret;
}

/* ────────────────────────────────────────
 * RC 수신 끊김 알람
 * 내림차순 부저
 * ──────────────────────────────────────── */
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
    buzSetPitch(DEF_BUZZER1, 0);
    ret = true;
  }
  return ret;
}

/* ────────────────────────────────────────
 * 저전압 알람
 * 단발 고음 부저
 * ──────────────────────────────────────── */
bool Alarm_LowBattery(void)
{
  bool ret = false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(200);
    buzSetPitch(DEF_BUZZER1, 0);
    ret = true;
  }
  return ret;
}

/* ────────────────────────────────────────
 * 페일세이프 알람
 * flag에 따라 알람 패턴 분기
 *   flag 1: RC 신호 이상 (Active Failsafe)
 *   flag 2: RC 수신 완전 끊김 (1초간 수신 없음)
 * ──────────────────────────────────────── */
bool Alarm_FailSafe(FailSafe_Flag flag)
{
  bool ret = false;
  if(ishwCan)
  {
    switch(flag)
    {
      case RC_FailSafe:  /* Active Failsafe - RC 신호 이상 */
        buzSetPitch(DEF_BUZZER1, 2000);
        delay(100);
        buzSetPitch(DEF_BUZZER1, 1000);
        delay(100);
        buzSetPitch(DEF_BUZZER1, 2000);
        delay(100);
        buzSetPitch(DEF_BUZZER1, 0);
        ledToggle(DEF_LED_2);
        ledToggle(DEF_LED_3);
        ret = true;
        break;

      case RC_Lost:  /* RC 수신 완전 끊김 */
        buzSetPitch(DEF_BUZZER1, 2000);
        delay(500);
        buzSetPitch(DEF_BUZZER1, 0);
        ledToggle(DEF_LED_2);
        ledToggle(DEF_LED_3);
        ret = true;
        break;

      default:
        break;
    }
  }
  return ret;
}

/* ────────────────────────────────────────
 * ESC 캘리브레이션 진입 알람
 * 오름차순 2회 부저
 * ──────────────────────────────────────── */
bool Alarm_EscCalibration(void)
{
  bool ret = false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 1500);
    delay(200);
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(200);
    buzSetPitch(DEF_BUZZER1, 0);
    delay(200);
    buzSetPitch(DEF_BUZZER1, 1500);
    delay(200);
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(200);
    buzSetPitch(DEF_BUZZER1, 0);
    ret = true;
  }
  return ret;
}

/* ────────────────────────────────────────
 * PID 게인 읽기 실패 알람
 * 오름차순 부저 + 전체 LED 토글
 * ──────────────────────────────────────── */
bool Alarm_Fail_PidRead(void)
{
  bool ret = false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 1000);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 1500);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 2000);
    delay(100);
    buzSetPitch(DEF_BUZZER1, 0);
    ledToggle(DEF_LED_1);
    ledToggle(DEF_LED_2);
    ledToggle(DEF_LED_3);
    ret = true;
  }
  return ret;
}

/* ────────────────────────────────────────
 * 아밍 경고 알람 (스로틀 안 내린 상태)
 * 단발 저음 반복 부저
 * ──────────────────────────────────────── */
bool Alarm_ArmingWarn(void)
{
  bool ret = false;
  if(ishwCan)
  {
    buzSetPitch(DEF_BUZZER1, 1000);
    delay(70);
    buzSetPitch(DEF_BUZZER1, 0);
    delay(70);
    ret = true;
  }
  return ret;
}
