/*
 * ap.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "ap.h"

LPS22HH_tbl_t LPS22HH;

void apInit(void)
{

  //uartOpen(DEF_UART4, 9600); //FC uart
    uartOpen(DEF_UART6, 9600); //PC uart
//  buzSetPitch(2000);
//  delay(1000);
//  buzSetPitch(1000);
//  delay(1000);
    buzDeinit();
    timDeinit();
}


void apMain(void)
{
  uint32_t premillis=0;
  LPS22HH_Flush(&LPS22HH);
  while(1)
  {
    if(millis()-premillis>=500)
    {
      ledToggle(DEF_LED_1);
      ledToggle(DEF_LED_2);
      ledToggle(DEF_LED_3);
     // printf("hello \n");
      premillis=millis();
    }
    ICM20602_GetInfo(AxisGyroRaw);
    LPS22HH_GetInfo(&LPS22HH,LPS22HH_GetPress);
  }
}
