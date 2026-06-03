/*
 * hw.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "hw.h"

void hwInit(void)
{
  bspInit();
  uartInit();
  cliInit();
  rtcInit();
  resetInit();
  flashInit();

  ledInit();
  spiInit();
  i2cInit();
  timInit();
  adcInit();

  M8N_Init();
  FSIA6B_Init();
  ROHS_Init();

  ICM20602_Init();
  LPS22HH_Init();
  BNO080_Init();
  AT24C08_Init();
  // buzInit();

}
