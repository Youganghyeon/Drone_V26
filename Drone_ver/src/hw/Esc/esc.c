/*
 * Oneshot.c
 *
 *  Created on: 2026. 6. 1.
 *      Author: yougang
 */


#include "tim.h"

#ifdef _USE_HW_ESC
typedef struct{
 isInit;

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

bool escCalibration(uint8_t ch)
{
  bool ret=false;
  for(int i=0; i<MAX_ESC_CH; i++)
  {
    TIM5->CCR1 = 21000;
    TIM5->CCR2 = 21000;
    TIM5->CCR3 = 21000;
    TIM5->CCR4 = 21000;
    HAL_Delay(7000);
    TIM5->CCR1 = 10500;
    TIM5->CCR2 = 10500;
    TIM5->CCR3 = 10500;
    TIM5->CCR4 = 10500;
    HAL_Delay(8000);

  }
  return ret;
}


void escWrite(uint8_t ch, uint32_t speed)
{

}
#endif
