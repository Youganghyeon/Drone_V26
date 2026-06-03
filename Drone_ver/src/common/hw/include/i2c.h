/*
 * i2c.h
 *
 *  Created on: 2026. 5. 20.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_I2C_H_
#define SRC_COMMON_HW_INCLUDE_I2C_H_

#include "hw_def.h"

#ifdef _USE_HW_I2C

#define MAX_I2C_CH      HW_MAX_I2C_CH
#define _DEF_I2C1       HW_DEF_I2C1



void i2cInit(void);
bool i2cOpen(uint8_t ch);
bool Isi2cInit(uint8_t ch);
bool Isi2cReady(uint8_t ch , uint16_t dev_addr);
bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t* pData, uint16_t Size);
bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *pData, uint16_t Size);
bool i2cWriteReg8B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t* pData, uint16_t length);
bool i2cReadReg8B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *pData, uint16_t length);
bool i2cWrite16B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t* pData, uint16_t length);
bool i2cRead16B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *pData, uint16_t length);

#endif
#endif /* SRC_COMMON_HW_INCLUDE_I2C_H_ */
