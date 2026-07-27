/*
 * bozzer.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "buzzer.h"
#include "tim.h"
#ifdef _USE_HW_BUZZER

static bool isInit = false;

typedef struct{
  bool     isInit;
  uint32_t psc;
}BUZZER_tbl;

bool buzInit(void)
{
  bool ret=true;
  for(int i=0; i<DEF_BUZZER_MAX_CH; i++)
  {
    timOpen(i);
  }
  if(timOpen(DEF_BUZZER1) == true)
  {
    pwmStart(DEF_BUZZER1);
    isInit = true;
  }
  return ret;
}

bool isBuzInit(void)
{
  return isInit;
}

bool buzSetPitch(uint8_t ch, uint32_t pitch)
{
  bool ret=false;
  switch(ch)
  {
    case DEF_BUZZER1:
      ret=timPsc(DEF_BUZZER1, pitch);
      break;
    default:
      break;
  }
  return ret;
}

bool buzDeinit(void)
{
  bool ret=true;
  pwmStop(DEF_BUZZER1);
  return ret;
}
#endif
