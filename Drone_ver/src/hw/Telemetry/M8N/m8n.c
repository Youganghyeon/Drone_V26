/*
 * m8n.c
 *
 *  Created on: 2026. 5. 21.
 *      Author: yougang
 */
#include "m8n.h"
#include "uart.h"
//UART5

#define DEF_M8N   DEF_UART4
#define M8N_Baudrate 115200

static bool isInit;
static bool isOpen;



const uint8_t UBX_CFG_PRT[] = {
  0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00,
  0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79
};

const uint8_t UBX_CFG_MSG[] = {
  0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x02, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x13, 0xBE
};

const uint8_t UBX_CFG_RATE[] = {
  0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00,
  0x01, 0x00, 0xDE, 0x6A
};

const uint8_t UBX_CFG_CFG[] = {
  0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x31,
  0xBF
};

void M8N_Init(void)
{
  isInit = false;
  isOpen = false;
}

bool M8N_Open(void)
{
  bool ret= false;
  if(IsUartOpen(DEF_M8N) == true)
  {
    ret=true;
  }else
  {
    ret=uartOpen(DEF_M8N, M8N_Baudrate);
  }
 if(ret == true)
 {
   isInit=true;
 }
 M8N_Transmit((uint8_t*)&UBX_CFG_PRT[0],sizeof(UBX_CFG_PRT));
 delay(100);
 M8N_Transmit((uint8_t*)&UBX_CFG_MSG[0],sizeof(UBX_CFG_MSG));
 delay(100);
 M8N_Transmit((uint8_t*)&UBX_CFG_RATE[0],sizeof(UBX_CFG_RATE));
 delay(100);
 M8N_Transmit((uint8_t*)&UBX_CFG_CFG[0],sizeof(UBX_CFG_CFG));
 delay(100);

 isOpen=true;
 return ret;
}


bool M8N_Transmit(uint8_t *buf, uint32_t length)
{
  return uartWrite(DEF_M8N, buf ,length);
}

uint8_t M8N_Receive(void)
{
  return uartRead(DEF_M8N);
}

bool M8N_ReceivePacket(M8N_tbl* m8n)
{
  M8N_UBX_RxBuf* p_RxBuf = &m8n->RxBuf;
  static uint8_t cnt=0;
  bool ret=false;
  if(uartAvailable(DEF_M8N)>0)
  {
    uint8_t rx_data;
    rx_data = M8N_Receive();
    switch(cnt)
    {
      case 0:
        if(rx_data == 0xb5)
        {
          p_RxBuf->buf[cnt] = rx_data;
          cnt++;
        }
        break;
      case 1:
        if(rx_data == 0x62)
        {
          p_RxBuf->buf[cnt] = rx_data;
          cnt++;
        }
        else
        {
          cnt = 0;
        }
         break;
      case 35:
        p_RxBuf->buf[cnt] = rx_data;
        cnt = 0;
        p_RxBuf->m8n_cplt_flag = 1;
        ret=true;
        break;
      default:
        p_RxBuf->buf[cnt] = rx_data;
        cnt++;
        break;
    }
  }
  return ret;
}

bool M8N_Parsing(uint8_t * data, M8N_UBX_NAV_POSLLH* posllh)
{
  posllh->CLASS  = data[2];
  posllh->ID     = data[3];
  posllh->length = data[4]  | data[5]<<8;
  posllh->itow   = data[6]  | data[7]<<8   | data[8]<<16  | data[9]<<24;
  posllh->lon    = data[10] | data[11]<<8  | data[12]<<16 | data[13]<<24;
  posllh->lat    = data[14] | data[15]<<8  | data[16]<<16 | data[17]<<24;
  posllh->height = data[18] | data[19]<<8  | data[20]<<16 | data[21]<<24;
  posllh->hMSL   = data[22] | data[23]<<8  | data[24]<<16 | data[25]<<24;
  posllh->hAcc   = data[26] | data[27]<<8  | data[28]<<16 | data[29]<<24;
  posllh->vAcc   = data[30] | data[31]<<8  | data[32]<<16 | data[33]<<24;
  return true;
}

bool MSN_UBX_CHKSUM_Check(uint8_t* data, uint8_t len)
{
  bool ret;
  uint8_t CK_A =0;
  uint8_t CK_B =0;
  for(int i=2; i<len-2; i++)
  {
    CK_A=CK_A+data[i];
    CK_B=CK_A+CK_B;
  }

  ret=(CK_A == data[len-2] && CK_B==data[len-1]);
  return ret;
}

