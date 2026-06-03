/*
 * reset.c
 *
 *  Created on: 2026. 6. 2.
 *      Author: yougang
 */

#include "reset.h"
#include "rtc.h"

#ifdef _USE_HW_RESET
static uint32_t reset_cnt=0;


void resetInit(void)
{
  if(isRtcInit() != true)
  {
    rtcInit();
  }
  if(((RCC->CSR) & (1U<<26)))
  {
    rtcBackupRegWrite(1, rtcBackupRegRead(1)+1);
    delay(500);
    reset_cnt = rtcBackupRegRead(1);
  }
  rtcBackupRegWrite(1, 0);
}

uint32_t getResetCount(void)
{
  return reset_cnt;
}
#endif
