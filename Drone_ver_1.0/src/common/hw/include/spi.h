/*
 * spi.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_SPI_H_
#define SRC_COMMON_HW_INCLUDE_SPI_H_

#include "hw_def.h"

#ifdef _USE_HW_SPI

#define DEF_SPI_ICM20602    DEF_HW_SPI_ICM20602
#define DEF_SPI_BNO080      DEF_HW_SPI_BNO080
void    spiInit(void);
bool    spiOpen(uint8_t ch);
bool    SPI_DMABytes(uint8_t ch, uint8_t *tx_data, uint8_t* rx_data, uint16_t length);
bool    SPI_PollByte(uint8_t ch, uint8_t *tx_data, uint8_t* rx_data, uint16_t length);
#endif



#endif /* SRC_COMMON_HW_INCLUDE_SPI_H_ */
