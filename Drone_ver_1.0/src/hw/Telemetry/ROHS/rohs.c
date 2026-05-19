/*
 * rohs.c
 *
 *  Created on: 2026. 5. 17.
 *      Author: yougang
 */


#include "rohs.h"
#include "uart.h"


void rohsInit(void)
{
 if(is_open[DEF_UART1] != true)
 {
   uartOpen(DEF_UART1);
 }
}

bool rohsRead(uint8_t ch)
{
  uint8_t rx_data;
  if(uartAvailable(DEF_UART1)>0)
  {
    rx_data=uartRead(DEF_UART1);
    rohsParsing(rx_data);
  }
}

bool rohsWrite(uint8_t* tx_data, uint32_t length)
{
  bool ret=false;
  ret = uartWrite(DEF_UART1, tx_data, length);
  return ret;
}
typedef enum{
  ROHS_Header_H=0,
  ROHS_Header_L=1,
  ROHS_Data=19
};


bool rohsParsing(uint8_t cmd)
{
  static uint8_t cnt = 0;
  switch(cnt)
  {
    case ROHS_Header_H:
      if(cmd == 0x47)
      {
        ROHS_rxbuf[cnt] = cmd;
        cnt++;
      }
     break;
    case ROHS_Header_L:
      if(cmd == 0x53)
      {
        ROHS_rxbuf[cnt] = cmd;
        cnt++;
      }
      else
      {
        cnt = 0;
      }
      break;
    case ROHS_Data:
      ROHS_rxbuf[cnt] = cmd;
      cnt = 0;
      ROHS_rxflag = 1;
      break;
    default:
      ROHS_rxbuf[cnt] = cmd;
      cnt++;
      break;
  }
}
