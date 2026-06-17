/*
 * FS-i6AB.c
 *
 *  Created on: 2026. 5. 25.
 *      Author: yougang
 */

//Only one

#include "fs-ia6b.h"
#include "uart.h"

#ifdef _USE_HW_FSIA6B
#define   DEF_FSI6AB        DEF_UART5
#define   FSI6AB_BAUDRATE   115200

void FSIA6B_Init(void)
{

}

bool FSIA6B_Open(FSiA6B_tbl* p_iBus)
{
  bool ret= false;
   if(IsUartOpen(DEF_FSI6AB) == true)
   {
     ret=true;
   }else
   {
     ret=uartOpen(DEF_FSI6AB, FSI6AB_BAUDRATE);
   }
   if(ret == true)
   {
     p_iBus->isOpen=true;
   }
   return ret;
}


uint8_t FSIA6B_Read(void)
{
  return uartRead(DEF_FSI6AB);
}

bool FSIA6B_RecivePacket(FSiA6B_tbl* iBus)
{
  static uint8_t cnt=0;
  bool ret=false;
  if(uartAvailable(DEF_FSI6AB)>0)
  {
    uint8_t rx_data;
    rx_data = FSIA6B_Read();
    switch(cnt)
    {
      case 0:
        if(rx_data == 0x20)
        {
          iBus->rxBuf[cnt] = rx_data;
          cnt++;
        }
        break;
      case 1:
        if(rx_data == 0x40)
        {
          iBus->rxBuf[cnt] = rx_data;
          cnt++;
        }
        else
          cnt = 0;
        break;
      case 31:
        iBus->rxBuf[cnt] = rx_data;
        cnt = 0;
        iBus->ibus_rx_cplt_flag = 1;
        break;
      default:
        iBus->rxBuf[cnt] = rx_data;
        cnt++;
        break;
    }
  }
  return ret;
}


bool FSIA6B_Check_checkSum(FSiA6B_tbl* iBus, uint8_t len)
{
  bool ret=false;
  iBus->checksum = 0xffff;

  for(int i=0;i<len-2;i++)
  {
    iBus->checksum = iBus->checksum - iBus->rxBuf[i];
  }
  if(((iBus->checksum & 0x00ff)==iBus->rxBuf[30]) && ((iBus->checksum>>8)==iBus->rxBuf[31]))
  {
    ret=true;
  }
  return ret;
}


void FSIA6B_Parsing(FSiA6B_tbl* iBus)
{
  iBus->RH = (iBus->rxBuf[2] | iBus->rxBuf[3]<<8) & 0x0fff;
  iBus->RV = (iBus->rxBuf[4] | iBus->rxBuf[5]<<8) & 0x0fff;
  iBus->LV = (iBus->rxBuf[6] | iBus->rxBuf[7]<<8) & 0x0fff;
  iBus->LH = (iBus->rxBuf[8] | iBus->rxBuf[9]<<8) & 0x0fff;
  iBus->SW[SW_A] = (iBus->rxBuf[10] | iBus->rxBuf[11]<<8) & 0x0fff;
  iBus->SW[SW_C] = (iBus->rxBuf[12] | iBus->rxBuf[13]<<8) & 0x0fff;
  iBus->FailSafe = (iBus->rxBuf[13]>>4);
}

bool FSIA6B_isFailsafe(FSiA6B_tbl* iBus)
{
  return iBus->FailSafe != 0;
}

#endif
