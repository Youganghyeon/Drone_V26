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
  timInit();
  buzInit();

}
