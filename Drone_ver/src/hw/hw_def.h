/*
 * hw_def.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "bsp.h"
#include "def.h"


#define _USE_HW_LED
#define _USE_HW_UART
#define _USE_HW_SPI
#define _USE_HW_TIM
#define _USE_HW_BUZZER
#define _USE_HW_M8N



#define HW_LED_MAX            3
#define HW_DEF_LED_1          0
#define HW_DEF_LED_2          1
#define HW_DEF_LED_3          2

#define HW_UART_MAX_CH        4
#define HW_DEF_UART1          0
#define HW_DEF_UART4          1
#define HW_DEF_UART5          2
#define HW_DEF_UART6          3


#define HW_PWM3_CH4           0

#define HW_SPI_MAX_CH         3
#define DEF_HW_ICM20602       0
#define DEF_HW_BNO080         1
#define DEF_HW_LPS22HH        2



#define HW_MAX_I2C_CH         1
#define HW_DEF_I2C1           0
#endif /* SRC_HW_HW_DEF_H_ */
