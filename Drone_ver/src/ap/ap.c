/*
 * ap.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "ap.h"


static LPS22HH_tbl_t LPS22HH;
static ICM20602_tbl_t ICM20602;
static BNO080_tbl BNO080;
extern uint8_t ROHS_rxbuf[20];


void apInit(void)
{

  //uartOpen(DEF_UART4, 9600); //FC uart
    uartOpen(DEF_UART6, 9600); //PC uart
    uartOpen(DEF_UART1, 115200); // ROHS
//  buzSetPitch(2000);
//  delay(1000);
//  buzSetPitch(1000);
//  delay(1000);
    buzDeinit();
    timDeinit();

    LPS22HH_Open(&LPS22HH);
    ICM20602_Open(&ICM20602);

    BNO080_Open(&BNO080);
    BNO080_enableRotationVector(&BNO080, 2500);

    AT24C08_Open();
}

float p = 0.0, i = 0.0, d = 0.0;
void apMain(void)
{
  uint32_t premillis=0;

  EP_PIDGain_Write(0, 1.1, 2.2, 3.3);
  while(1)
  {
    if(millis()-premillis>=500)
    {
      ledToggle(DEF_LED_1);
      ledToggle(DEF_LED_2);
      ledToggle(DEF_LED_3);
      premillis=millis();
    }

//    LPS22HH_GetInfo(&LPS22HH,LPS22HH_GetPress);
//
//    ICM20602_GetInfo(&ICM20602, AxisGyroRaw);
//
//    BNO080_ReadInfo(&BNO080);
    EP_PIDGain_Read(0, &p, &i, &d);



    /*
     * rohsRead();
     *
     */



  }
}
