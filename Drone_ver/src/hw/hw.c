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
  i2cInit();
 // timInit();
 // buzInit();



  spiOpen(DEF_HW_ICM20602);
  spiOpen(DEF_HW_LPS22HH);
  spiOpen(DEF_HW_BNO080);
  i2cOpen(HW_DEF_I2C1);

  M8N_Init();
  ICM20602_Init();
  LPS22HH_Init();
  BNO080_Init();
  AT24C08_Init();

//  rohsInit();
}
