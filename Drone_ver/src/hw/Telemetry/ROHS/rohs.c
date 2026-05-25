/*
 * rohs.c
 *
 *  Created on: 2026. 5. 17.
 *      Author: yougang
 */


#include "rohs.h"
#include "uart.h"

//#define   DEF_ROHS_UART     DEF_UART1
#define   DEF_ROHS_BAUD     115200


void ROHS_Init(void)
{
}

bool ROHS_Open(ROHS_tbl* p_rohs, uint8_t uart_ch)
{
  bool ret= false;
  if(IsUartOpen(uart_ch) != true)
  {
    ret = true;
  }
  else
  {
    ret=uartOpen(uart_ch, DEF_ROHS_BAUD);
  }

  if(ret==true)
  {
    p_rohs->uartCh = uart_ch;
    p_rohs->isOpen = true;
  }
  return ret;
}

bool ROHS_Read(ROHS_tbl* p_rohs)
{
  bool ret=false;
  uint8_t rx_data;
  if(uartAvailable(p_rohs->uartCh)>0)
  {
    rx_data=uartRead(p_rohs->uartCh);
    ROHS_Parsing(p_rohs,rx_data);
    ret=true;
  }
   return ret;
}

bool ROHS_Write(ROHS_tbl* p_rohs, uint8_t* tx_data, uint32_t length)
{
  bool ret=false;
  ret = uartWrite(p_rohs->uartCh, tx_data, length);
  return ret;
}



bool ROHS_Parsing(ROHS_tbl* p_rohs, uint8_t cmd)
{
  bool ret=false;
  static uint8_t cnt = 0;
  switch(cnt)
  {
    case ROHS_Header_H:
      if(cmd == 0x47)
      {
        p_rohs->rxBuf[cnt] = cmd;
        cnt++;
        p_rohs->rx_status = ROHS_Header_H;
      }
     break;
    case ROHS_Header_L:
      if(cmd == 0x53)
      {
        p_rohs->rxBuf[cnt] = cmd;
        p_rohs->rx_status = ROHS_Header_L;
        cnt++;
      }
      else
      {
        cnt = 0;
      }
      break;
    case ROHS_Complete:
      p_rohs->rxBuf[cnt] = cmd;
      cnt = 0;
      p_rohs->rx_cpltFlag = 1;
      p_rohs->rx_status = ROHS_Complete;
      ret=true;
      break;
    default:
      p_rohs->rxBuf[cnt] = cmd;
      p_rohs->rx_status = ROHS_PayLoad;
      cnt++;
      break;
  }
  return ret;
}

