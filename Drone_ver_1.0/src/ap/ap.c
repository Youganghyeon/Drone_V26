/*
 * ap.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "ap.h"


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
    ICM20602_Read(AxisGyroRaw);

  }
}
