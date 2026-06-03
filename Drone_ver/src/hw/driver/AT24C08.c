/*
 * AT24C08.c
 *
 *  Created on: 2026. 5. 20.
 *      Author: yougang
 */


#include "AT24C08.h"
#include "i2c.h"

#ifdef _USE_HW_AT24C08
#define DEF_AT24C08   HW_DEF_I2C1
#define AT24C08_ADDR  0x50
//#define AT24C08_ADDR_HAL (0x50<<1 = 0xA0)

static bool isInit = false;
static bool isOpen = false;
static bool AT24C08_PageWrite(uint8_t page, uint8_t* data, uint8_t len);
static bool AT24C08_PageRead(uint8_t page, uint8_t* data, uint8_t len);
typedef union{
  float f;
  uint8_t byte[4];
}Parser;


bool AT24C08_isInit(void)
{
  return isInit;
}


bool AT24C08_isOpen(void)
{
  return isOpen;
}


void AT24C08_Init(void)
{
 if(Isi2cInit(DEF_AT24C08) == true)
 {
  isInit=true;
 }
 else
 {
   i2cOpen(DEF_AT24C08);
 }
}

bool AT24C08_Open(void)
{
  bool ret=false;
  if(Isi2cReady(DEF_AT24C08, AT24C08_ADDR) == true)
  {
    if(isInit == true)
    {
      isOpen = true;
      ret=true;
    }
  }
  return ret;
}

bool AT24C08_PageWrite(uint8_t page, uint8_t* data, uint8_t len)
{
  bool ret=false;
  uint8_t dev_addr = ((page*16)>>8)| (AT24C08_ADDR);
  uint8_t word_addr = (page*16) & 0xff;
  HAL_GPIO_WritePin(I2C1_WP_GPIO_Port,I2C1_WP_Pin, GPIO_PIN_RESET);
  ret = i2cWriteReg8B(DEF_AT24C08, dev_addr, word_addr, data, len);
  delay(1);
  HAL_GPIO_WritePin(I2C1_WP_GPIO_Port,I2C1_WP_Pin, GPIO_PIN_SET);

  return ret;
}
bool AT24C08_PageRead(uint8_t page, uint8_t* data, uint8_t len)
{
  bool ret=false;
  uint8_t dev_addr = ((page*16)>>8)| (AT24C08_ADDR);
  uint8_t word_addr = (page*16) & 0xff;
  ret = i2cReadReg8B(DEF_AT24C08, dev_addr, word_addr, data, len);
  delay(1);
  return ret;
}
void EP_PIDGain_Write(uint8_t id, float PGain, float IGain, float DGain)
{
  uint8_t buf_write[16];
  Parser parser;

  buf_write[0] = 0x45;
  buf_write[1] = 0x50;
  buf_write[2] = id;
  parser.f = PGain;
  buf_write[3] = parser.byte[0];
  buf_write[4] = parser.byte[1];
  buf_write[5] = parser.byte[2];
  buf_write[6] = parser.byte[3];

  parser.f = IGain;
  buf_write[7] = parser.byte[0];
  buf_write[8] = parser.byte[1];
  buf_write[9] = parser.byte[2];
  buf_write[10] = parser.byte[3];

  parser.f = DGain;
  buf_write[11] = parser.byte[0];
  buf_write[12] = parser.byte[1];
  buf_write[13] = parser.byte[2];
  buf_write[14] = parser.byte[3];

  unsigned char chksum = 0xff;
  for(int i=0;i<15;i++)
  {
    chksum -= buf_write[i];

  }

  buf_write[15] = chksum;

  switch(id)
  {
  case 0:
    AT24C08_PageWrite(0, &buf_write[0], 16);
    break;
  case 1:
    AT24C08_PageWrite(1, &buf_write[0], 16);
    break;
  case 2:
    AT24C08_PageWrite(2, &buf_write[0], 16);
    break;
  case 3:
    AT24C08_PageWrite(3, &buf_write[0], 16);
    break;
  case 4:
    AT24C08_PageWrite(4, &buf_write[0], 16);
    break;
  case 5:
    AT24C08_PageWrite(5, &buf_write[0], 16);
    break;
  }
}

bool EP_PIDGain_Read(PID_Angle id, float* PGain, float* IGain, float* DGain)
{
  unsigned char buf_read[16];
  Parser parser;
  if(id==PID_All)
  {
    for(int i=0; i<id; i++)
    {
      AT24C08_PageRead((uint8_t)i, &buf_read[0], 16);
    }
  }
  AT24C08_PageRead((uint8_t)id, &buf_read[0], 16);
  uint8_t chksum = 0xff;
  for(int i=0;i<15;i++) chksum -= buf_read[i];

  if(buf_read[15] == chksum && buf_read[0] == 0x45 && buf_read[1] == 0x50)
  {
    parser.byte[0] = buf_read[3];
    parser.byte[1] = buf_read[4];
    parser.byte[2] = buf_read[5];
    parser.byte[3] = buf_read[6];
    *PGain = parser.f;

    parser.byte[0] = buf_read[7];
    parser.byte[1] = buf_read[8];
    parser.byte[2] = buf_read[9];
    parser.byte[3] = buf_read[10];
    *IGain = parser.f;

    parser.byte[0] = buf_read[11];
    parser.byte[1] = buf_read[12];
    parser.byte[2] = buf_read[13];
    parser.byte[3] = buf_read[14];
    *DGain = parser.f;

    return true;
  }

  return false;
}
#endif
