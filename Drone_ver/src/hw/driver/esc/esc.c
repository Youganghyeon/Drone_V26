/*
 * Oneshot.c
 *
 *  Created on: 2026. 6. 1.
 *      Author: yougang
 */


#include "tim.h"

#ifdef _USE_HW_ESC

#define ESC_IDLE_SPEED    10500
#define ESC_MAX_SPEED     21000


typedef struct{
 bool     isInit;
 uint32_t speed;

}Esc_tbl;

Esc_tbl Esc[MAX_ESC_CH];
static bool isOpen[MAX_ESC_CH];

void escInit(void)
{
 for(int i=0; i<MAX_ESC_CH; i++)
 {
   isOpen=false;
 }
}
void escOpen(uint8_t ch)
{
  bool ret=false;
  if(isTimOpen()==true)
  {
    ret=true;
  }
  else
  {
    ret=timOpen();
    isOpen[ch]=ret;
  }
  return ret;
}

bool escCalibration(void)
{
  for(int i=0; i<MAX_ESC_CH; i++)
  {
    pwmChange(i, 21000);
    delay(7000);
    pwmChange(i, 21000);
    delay(8000);
  }
  return true;
}


bool escOutput(uint8_t ch, uint32_t speed)
{
 bool ret = false;
 if(speed > ESC_MAX_SPEED)
 {
   speed = ESC_MAX_SPEED
 }
 if(speed < ESC_IDLE_SPEED)
 {
   speed = ESC_IDLE_SPEED
 }
  ret = pwmChange(DEF_TIM5_CH1, speed);
 return ret;
}
#endif
