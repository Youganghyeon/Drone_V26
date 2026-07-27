/*
 * Oneshot.c
 *
 *  Created on: 2026. 6. 1.
 *      Author: yougang
 */

#include "esc.h"
#include "tim.h"

#ifdef _USE_HW_ESC

typedef struct{
 bool     isInit;
 uint32_t speed;

}Esc_tbl;

Esc_tbl Esc[ESC_MAX_CH];

void escInit(void)
{
  for(int i=0; i<ESC_MAX_CH; i++)
  {
    Esc[i].isInit = false;
  }
}
bool escOpen(uint8_t ch)
{
  bool ret=false;
  if(istimOpen(ch)==true)
  {
    ret=true;
    Esc[ch].isInit=true;
  }
  else
  {
    ret=timOpen(ch);
    Esc[ch].isInit=ret;
  }
  return ret;
}

bool escCalibration(void)
{
  for(int i=0; i<ESC_MAX_CH; i++)
  {
    pwmChange(i, 21000);
  }

  delay(7000);

  for(int i=0; i<ESC_MAX_CH; i++)
  {
    pwmChange(i, 10500);
  }
  delay(8000);

  return true;
}


bool escOutput(uint8_t ch, uint32_t speed)
{
 bool ret = false;
 if(speed > ESC_MAX_SPEED)
 {
   speed = ESC_MAX_SPEED;
 }
 if(speed < ESC_IDLE_SPEED)
 {
   speed = ESC_IDLE_SPEED;
 }
  ret = pwmChange(ch, speed);
 return ret;
}
#endif
