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
  ledInit();
  spiInit();
 // timInit();
 // buzInit();
  spiOpen(DEF_HW_ICM20602);
  spiOpen(DEF_HW_LPS22HH);
  ICM20602_Init();
  LPS22HH_Init();
}
