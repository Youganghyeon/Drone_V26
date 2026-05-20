/*
 * LPS22H.c
 *
 *  Created on: 2026. 5. 15.
 *      Author: yougang
 */


/**
 * LPS22HH.c
 * @author ChrisP @ M-HIVE

 * This library source code has been created for STM32F4. Only supports SPI.
 *
 * Development environment specifics:
 * STM32CubeIDE 1.0.0
 * STM32CubeF4 FW V1.24.1
 * STM32F4 LL Driver(SPI)
 *
 * Created by ChrisP(Wonyeob Park) @ M-HIVE Embedded Academy, July, 2019
 * Rev. 1.0
 *
 * https://github.com/ChrisWonyeobPark
 * https://blog.naver.com/lbiith
 * https://cafe.naver.com/mhiveacademy
*/

#include "LPS22HH/LPS22HH.h"
#include "spi.h"


/**
 * @brief LPS22HH Register Map
 */
#define CHIP_SELECT(LPS22HH)        HAL_GPIO_WritePin(LPS22HH_SPI_CS_PORT, LPS22HH_SPI_CS_PIN, RESET)
#define CHIP_DESELECT(LPS22HH)      HAL_GPIO_WritePin(LPS22HH_SPI_CS_PORT, LPS22HH_SPI_CS_PIN, SET)

#define INTERRUPT_CFG 0x0B
#define THS_P_L     0x0C
#define THS_P_H     0x0D
#define IF_CTRL     0x0E
#define WHO_AM_I    0x0F
#define CTRL_REG1   0x10
#define CTRL_REG2   0x11
#define CTRL_REG3   0x12
#define FIFO_CTRL   0x13
#define FIFO_WTM    0x14
#define REF_P_L     0x15
#define REF_P_H     0x16
//  Reserved      0x17
#define RPDS_L      0x18
#define RPDS_H      0x19
//  Reserved      0x1A-0x23
#define INT_SOURCE    0x24
#define FIFO_STATUS1  0x25
#define FIFO_STATUS2  0x26
#define STATUS      0x27
#define PRESSURE_OUT_XL 0x28
#define PRESSURE_OUT_L  0x29
#define PRESSURE_OUT_H  0x2A
#define TEMP_OUT_L    0x2B
#define TEMP_OUT_H    0x2C
//  Reserved      0x2D-0x77
#define FIFO_DATA_OUT_PRESS_XL  0x78
#define FIFO_DATA_OUT_PRESS_L 0x79
#define FIFO_DATA_OUT_PRESS_H 0x7A
#define FIFO_DATA_OUT_TEMP_L  0x7B
#define FIFO_DATA_OUT_TEMP_H  0x7C


#define  DEF_LPS22HH    DEF_HW_LPS22HH
static   SPI_DMA_STATE LPS22HH_Flag=IDLE;


static   void LPS22HH_RxFunc(void);

bool LPS22HH_Readbyte(uint8_t reg_addr, uint8_t* rx_data)
{
  bool ret=false;
  uint8_t tx_data = (reg_addr | 0x80);
  uint8_t dummy, rx_dummy = 0x00;
  CHIP_SELECT(LPS22HH);
  ret=SPI_SendReceive(DEF_LPS22HH, &tx_data, &rx_dummy, 1);
  ret=SPI_SendReceive(DEF_LPS22HH, &dummy, rx_data, 1);
  CHIP_DESELECT(LPS22HH);
  return ret;
}

bool LPS22HH_Readbytes_DMA(uint8_t reg_addr, uint8_t* rx_data, uint16_t len)
{
  bool ret=false;
  uint8_t tx_data = (reg_addr | 0x80);
  uint8_t dummy, rx_dummy = 0x00;
  CHIP_SELECT(LPS22HH);
  ret=SPI_SendReceive(DEF_LPS22HH, &tx_data, &rx_dummy, 1);
  ret=SPI_SendReceive_DMA(DEF_LPS22HH, &dummy, rx_data, len);
  return ret;
}

void LPS22HH_Writebyte(uint8_t reg_addr, uint8_t* val)
{
  uint8_t rx_dummy;
  uint8_t tx_data=(reg_addr & 0x7F);
  CHIP_SELECT(LPS22HH);
  SPI_SendReceive(DEF_LPS22HH,&tx_data, &rx_dummy,1); //Register. MSB 0 is write instruction.
  SPI_SendReceive(DEF_LPS22HH,val, &rx_dummy,1); //Data
  CHIP_DESELECT(LPS22HH);
}

/*
void LPS22HH_Writebytes(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
  unsigned int i = 0;
  CHIP_SELECT(LPS22HH);
  SPI3_SendByte(reg_addr & 0x7F); //Register. MSB 0 is write instruction.
  while(i < len)
  {
    SPI3_SendByte(data[i++]); //Data
  }
  CHIP_DESELECT(LPS22HH);
}
*/
bool LPS22HH_Init(void)
{
 return true;
}


bool LPS22HH_Open(LPS22HH_tbl_t* p_sensor)
{
  uint8_t temp_reg;
  uint8_t who_am_i = 0;

  if(IsSpiInit(DEF_LPS22HH) != false)
  {
    spiOpen(DEF_LPS22HH);
  }

  printf("Checking LPS22HH...");

  // check WHO_AM_I (0x0F)
  LPS22HH_Readbyte(0x0F,&who_am_i);

  // who am i = 0xb3
  if( who_am_i == 0xb3)
  {
    printf("\nLPS22HH who_am_i = 0x%02x...OK\n\n", who_am_i );
  }
  else if( who_am_i != 0xb3)
  {
    LPS22HH_Readbyte(0x0F, &who_am_i); // check WHO_AM_I (0x0F)

    if ( who_am_i != 0xb3 ){
      printf( "nLPS22HH Not OK: 0x%02x Should be 0x%02x\n", who_am_i, 0xb3);
      return false; //ERROR
    }
  }
  spiRxCallbackRegister(DEF_LPS22HH, LPS22HH_RxFunc);
  // Reset LPS22HH
  // CTRL_REG2 0x11
  uint8_t CTRL_REG2_val=0x04;
  LPS22HH_Writebyte(CTRL_REG2, &CTRL_REG2_val);
  //printf("LPS22HH Reset");
  uint32_t pre_time=millis();
  uint8_t reg_reset;
  while(1)
  {
    LPS22HH_Readbyte((CTRL_REG2 & 0x04), &reg_reset);
    if(reg_reset == 0x00)
    {
      break;
    }
    if(millis()-pre_time>=10000) //10s
    {
      printf("LPS22HH Reset fail\n");
      return false;
    }
  }
  //printf("Complete\n");

  // Set Output Data Rate
  //0x00: One Shot
  //0x10: 1Hz 0x20: 10Hz  0x30: 25Hz  0x40: 50Hz
  //0x50: 75Hz  0x60: 100Hz 0x70: 200Hz
  LPS22HH_Readbyte(CTRL_REG1, &temp_reg);
  temp_reg = temp_reg | 0x40;
  LPS22HH_Writebyte(CTRL_REG1, &temp_reg);
  temp_reg = 0;
  LPS22HH_Readbyte(CTRL_REG1, &temp_reg);
  //printf("%x\n", temp_reg);

  // Enable LPF, Cut-off frequency
  //0x08: ODR/9 0x0c: ODR/20
  LPS22HH_Readbyte(CTRL_REG1, &temp_reg);
  temp_reg = temp_reg | 0x0c;
  LPS22HH_Writebyte(CTRL_REG1, &temp_reg);

  // Enable Block Data Update
  LPS22HH_Readbyte(CTRL_REG1, &temp_reg);
  temp_reg = temp_reg | 0x02;
  LPS22HH_Writebyte(CTRL_REG1, &temp_reg);

  // Enable Low Noise Mode (ODR should be lower than 100Hz. This is igonored when ODR = 100Hz or 200Hz)
  LPS22HH_Readbyte(CTRL_REG2, &temp_reg);
  temp_reg = temp_reg | 0x02;
  LPS22HH_Writebyte(CTRL_REG2, &temp_reg);

  // Enable Data-ready signal on INT-DRDY pin
  LPS22HH_Readbyte(CTRL_REG3, &temp_reg);
  temp_reg = temp_reg | 0x04;
  LPS22HH_Writebyte(CTRL_REG3, &temp_reg);

  p_sensor->isOpen=true;
  return true; //OK
}

void LPS22HH_RxFunc(void)
{
  LPS22HH_Flag = DONE;
  CHIP_DESELECT(LPS22HH);
}

#define X 0.90f
bool LPS22HH_GetInfo(LPS22HH_tbl_t* p_sensor, uint32_t mode)
{
  if(LPS22HH_DataReady() == 1 &&(LPS22HH_Flag==IDLE))
  {
    switch(mode)
     {
       case LPS22HH_GetPress:
         LPS22HH_Readbytes_DMA(PRESSURE_OUT_XL, (uint8_t*)&p_sensor->pressure_raw, 3);
         LPS22HH_Flag = Active;
         break;
       case LPS22HH_GetTemp:
         LPS22HH_Readbytes_DMA(TEMP_OUT_L, (uint8_t*)&p_sensor->temperature_raw, 2);
         LPS22HH_Flag = Active;
     }
  }
  if(LPS22HH_Flag==DONE)
  {
    p_sensor->baroAlt = getAltitude2((p_sensor->pressure_raw)/4096.f, (p_sensor->temperature_raw)/100.f);
    p_sensor->baroAltFilt = (p_sensor->baroAltFilt) * X + (p_sensor->baroAlt) * (1.0f - X);
    LPS22HH_Flag = IDLE;
  }
  return true;
}

uint8_t LPS22HH_State(void)
{
  return LPS22HH_Flag;
}

bool LPS22HH_DataReady(void)
{
  bool ret=false;
  if((HAL_GPIO_ReadPin(LPS22HH_INT_PORT, LPS22HH_INT_PIN) == GPIO_PIN_SET))
  {
    ret=true;
  }
  return ret;
}

bool LPS22HH_Flush(LPS22HH_tbl_t* p_sensor)
{
  p_sensor->temperature_raw=0;
  p_sensor->pressure_raw=0;
  p_sensor->baroAlt=0.0;
  p_sensor->baroAltFilt=0.0;
  return true;
}
#define SEA_PRESSURE 1013.25f

float getAltitude1(float pressure) //No temperature correction.
{
  return (powf((SEA_PRESSURE / pressure), 0.1902226f) - 1.0) * 44307.69396f; //145366.45f * 0.3048f = 44307.69396f;
}

float getAltitude2(float pressure, float temperature) //Get Altitude with temperature correction.
{
  return ((powf((SEA_PRESSURE / pressure), 0.1902226f) - 1.0f) * (temperature + 273.15f)) / 0.0065f;
}
