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
#define _USE_HW_RTC
#define _USE_HW_UART
#define _USE_HW_SPI
#define _USE_HW_TIM
#define _USE_HW_ADC
//#define _USE_HW_I2C
#define _USE_HW_RESET
#define _USE_HW_FLASH
//#define _USE_HW_ESC
//#define _USE_HW_BUZZER
//#define _USE_HW_ICM20602
//#define _USE_HW_BNO080
//#define _USE_HW_M8N
//#define _USE_HW_AT24C08
//#define _USE_HW_LPS22HH
#define _USE_HW_FSIA6B
#define _USE_HW_ROHS
#define _USE_HW_CMD
#define _USE_HW_BOOT

//#define _USE_HW_SENSOR

#define HW_LED_MAX            3
#define HW_DEF_LED_1          0
#define HW_DEF_LED_2          1
#define HW_DEF_LED_3          2

#define HW_UART_MAX_CH        4

#define HW_DEF_UART1          0
#define HW_DEF_UART4          1
#define HW_DEF_UART5          2
#define HW_DEF_UART6          3
/*----------TIM-------------------*/
#define HW_TIM_TYPE_PWM_FLAG    (0x00)
#define HW_TIM_TYPE_TIMER_FLAG  (0x80)
#define HW_TIM_IS_TIMER(ch)     (ch & 0x80)
#define HW_TIM_GET_INDEX(ch)    (ch & 0x7F)
#define HW_TIM_MAX_CH           (HW_PWM_MAX_CH+HW_TIMER_MAX_CH)

#define HW_PWM_MAX_CH           5
#define HW_DEF_TIM3_CH4         0
#define HW_DEF_TIM5_CH1         1
#define HW_DEF_TIM5_CH2         2
#define HW_DEF_TIM5_CH3         3
#define HW_DEF_TIM5_CH4         4

#define HW_TIMER_MAX_CH         1
#define HW_DEF_TIM7             0

#define DEF_TIM3_CH4           (HW_TIM_TYPE_PWM_FLAG   | HW_DEF_TIM3_CH4)
#define DEF_TIM5_CH1           (HW_TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH1)
#define DEF_TIM5_CH2           (HW_TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH2)
#define DEF_TIM5_CH3           (HW_TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH3)
#define DEF_TIM5_CH4           (HW_TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH4)
#define DEF_TIM7               (HW_TIM_TYPE_TIMER_FLAG | HW_DEF_TIM7)
/*------------------------------*/

#define HW_SENSOR_MAX_CH      3
#define HW_SPI_MAX_CH         3
#define DEF_HW_ICM20602       0
#define DEF_HW_BNO080         1
#define DEF_HW_LPS22HH        2
#define HW_ADC_MAX_CH         1
#define HW_DEF_ADC1           0

#define HW_MAX_I2C_CH         1
#define HW_DEF_I2C1           0

#define HW_DEF_FSIA6B_SW_MAX_CH 4
#define HW_DEF_FSIA6B_SW_A      0
#define HW_DEF_FSIA6B_SW_B      1
#define HW_DEF_FSIA6B_SW_C      2
#define HW_DEF_FSIA6B_SW_D      3


#define HW_CMD_MAX_DATA_LENGTH  1024

#endif /* SRC_HW_HW_DEF_H_ */
