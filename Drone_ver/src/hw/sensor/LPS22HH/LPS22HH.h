/*
 * LPS22H.h
 *
 *  Created on: 2026. 5. 15.
 *      Author: yougang
 */

#ifndef SRC_HW_SENSOR_LPS22H_LPS22HH_H_
#define SRC_HW_SENSOR_LPS22H_LPS22HH_H_

#include "hw_def.h"
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
/*
SPI Operational Features
1. Data is delivered MSB first and LSB last
2. Data is latched on the rising edge of SPC
3. Data should be transitioned on the falling edge of SPC
4. The maximum frequency of SPC is 10MHz
5. SPI read and write operations are completed in 16 or more clock cycles (two or more bytes). The first byte contains the
SPI Address, and the following byte(s) contain(s) the SPI data. The first bit of the first byte contains the Read/Write bit
and indicates the Read (1) or Write (0) operation. The following 7 bits contain the Register Address. In cases of multiplebyte Read/Writes, data is two or more bytes:
*/

/**
 * @brief Definition for connected to SPI3 (APB1 = 42MHz)
 */
#define LPS22HH_SPI_CHANNEL     SPI3

#define LPS22HH_SPI_CS_PIN      GPIO_PIN_6
#define LPS22HH_SPI_CS_PORT     GPIOB
#define LPS22HH_SPI_CS_CLK      AHB1_GRP1_PERIPH_GPIOB

#define LPS22HH_INT_PIN       GPIO_PIN_7
#define LPS22HH_INT_PORT      GPIOB
#define LPS22HH_INT_CLK       AHB1_GRP1_PERIPH_GPIOB
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define LPS22HH_GetPress       0
#define LPS22HH_GetTemp        1
#define LPS22HH_GetAll         2

typedef enum{
  NO_TEMP_CORRECT = 0,
  TEMP_CORRECT       ,
}LPS22HH_GetALT_Mode;

typedef struct {
  int32_t pressure_raw;
  int16_t temperature_raw;
  float baroAlt;
  float baroAltFilt;
  bool isOpen;
}LPS22HH_tbl_t;

bool LPS22HH_Init(void);
bool LPS22HH_Open(LPS22HH_tbl_t* p_sensor);
bool LPS22HH_DataReady(void);
uint8_t LPS22HH_State(void);
bool   LPS22HH_GetInfo(LPS22HH_tbl_t* p_sensor, uint32_t mode);
bool LPS22HH_Flush(LPS22HH_tbl_t* p_sensor);
void LPS22HH_GetPressure(int32_t* pressure);
void LPS22HH_GetTemperature(int16_t* temperature);
bool LPS22HH_GetAlt(LPS22HH_tbl_t* p_sensor, LPS22HH_GetALT_Mode mode);
bool LPS22HH_GetAltFilt(LPS22HH_tbl_t* p_sensor);



#endif /* SRC_HW_SENSOR_LPS22H_LPS22HH_H_ */
