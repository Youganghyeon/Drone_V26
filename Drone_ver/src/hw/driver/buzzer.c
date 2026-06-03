/*
 * bozzer.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "buzzer.h"
#include "tim.h"
#ifdef _USE_HW_BUZZER
#define  DEF_BUZZER  DEF_TIM3_CH4

bool buzInit(void)
{
  bool ret=true;
  pwmStart(DEF_BUZZER);
  return ret;
}


bool buzSetPitch(uint32_t pitch)
{
  bool ret=true;
  timPsc(DEF_BUZZER, pitch);
  return ret;
}

bool buzDeinit(void)
{
  bool ret=true;
  pwmStop(DEF_BUZZER);
  return ret;
}
#endif
