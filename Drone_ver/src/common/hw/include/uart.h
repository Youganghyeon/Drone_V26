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

#define UART_MAX_CH     HW_UART_MAX_CH
#define DEF_UART1       HW_DEF_UART1
#define DEF_UART4       HW_DEF_UART4
#define DEF_UART5       HW_DEF_UART5
#define DEF_UART6       HW_DEF_UART6

bool        uartInit(void);
bool        uartOpen(uint8_t ch, uint32_t baud);
bool        IsUartOpen(uint8_t ch);
uint32_t    GetBaud(uint8_t ch);
uint32_t    uartWrite(uint8_t ch, uint8_t *buf, uint32_t length);
uint8_t     uartRead(uint8_t ch);
uint32_t    uartWrite(uint8_t ch, uint8_t *buf, uint32_t length);
uint32_t    uartAvailable(uint8_t ch);
uint32_t    uartPrintf(uint8_t ch, const char* fmt,...);
#endif

#endif /* SRC_COMMON_HW_INCLUDE_UART_H_ */
