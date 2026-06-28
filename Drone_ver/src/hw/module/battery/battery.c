/*
 * battery.c
 *
 *  Created on: 2026. 6. 28.
 *      Author: yougang
 */

#include "battery.h"
#include "adc.h"
bool isInit;


static float    BatVolt=0.0f;
static uint16_t adcVolt=0;


void batInit(void)
{
  isInit = false;
}


bool batStart(void)
{
  if(adcReceive_DMA(DEF_ADC1, (uint32_t*)&adcVolt, 1))
  {
    isInit = true;
  }
  return isInit;
}

bool isBatInit(void)
{
  return isInit;
}
float batCheck(void)
{
  if(isInit == true)
  {
    BatVolt = adcVolt * 0.003619f;
  }
  else
  {
    BatVolt = 0.0f;
  }
  return BatVolt;
}
