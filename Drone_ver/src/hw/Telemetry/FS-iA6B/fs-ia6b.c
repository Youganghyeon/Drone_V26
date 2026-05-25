/*
 * FS-i6AB.c
 *
 *  Created on: 2026. 5. 25.
 *      Author: yougang
 */

//Only one

#include "fs-ia6b.h"
#include "uart.h"
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


bool FSIA6B_Check_checkSum(uint8_t* data, uint8_t len)
{
  bool ret=false;
  uint16_t chksum = 0xffff;

  for(int i=0;i<len-2;i++)
  {
    chksum = chksum - data[i];
  }
  if(((chksum&0x00ff)==data[30]) && ((chksum>>8)==data[31]))
  {
    ret=true;
  }
  return ret;
}


void FSIA6B_Parsing(uint8_t* data, FSiA6B_tbl* iBus)
{
  iBus->RH = (data[2] | data[3]<<8) & 0x0fff;
  iBus->RV = (data[4] | data[5]<<8) & 0x0fff;
  iBus->LV = (data[6] | data[7]<<8) & 0x0fff;
  iBus->LH = (data[8] | data[9]<<8) & 0x0fff;
  iBus->SwA = (data[10] | data[11]<<8) & 0x0fff;
  iBus->SwC = (data[12] | data[13]<<8) & 0x0fff;
  iBus->FailSafe = (data[13] >> 4);
}

bool FSIA6B_isFail(FSiA6B_tbl* iBus)
{
  return iBus->FailSafe != 0;
}




