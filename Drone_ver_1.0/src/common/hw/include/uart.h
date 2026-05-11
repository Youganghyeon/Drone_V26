/*
 * uart.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_UART_H_
#define SRC_COMMON_HW_INCLUDE_UART_H_

#include "hw_def.h"

#ifdef _USE_HW_UART

bool uartInit(void);
bool uartOpen(uint8_t ch, uint32_t baud);
uint32_t GetBaud(uint8_t ch);


#define UART_MAX_CH     HW_UART_MAX_CH
#define DEF_UART4       HW_DEF_UART4
#define DEF_UART6       HW_DEF_UART6
#endif

#endif /* SRC_COMMON_HW_INCLUDE_UART_H_ */
