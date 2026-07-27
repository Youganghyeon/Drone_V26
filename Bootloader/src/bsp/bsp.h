/*
 * bsp.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"
#include "stm32f4xx_hal.h"

void bspDeinit(void);
void bspInit(void);
void delay(uint32_t ms);
uint32_t millis(void);
void Error_Handler(void);
void SystemClock_Config(void);

#define I2C1_WP_Pin GPIO_PIN_13
#define I2C1_WP_GPIO_Port GPIOC
#define LED_1_Pin GPIO_PIN_0
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_1
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin GPIO_PIN_2
#define LED_3_GPIO_Port GPIOC
#define ESC_1_Pin GPIO_PIN_0
#define ESC_1_GPIO_Port GPIOA
#define ESC_2_Pin GPIO_PIN_1
#define ESC_2_GPIO_Port GPIOA
#define ESC_3_Pin GPIO_PIN_2
#define ESC_3_GPIO_Port GPIOA
#define ESC_4_Pin GPIO_PIN_3
#define ESC_4_GPIO_Port GPIOA
#define ICM_CS_Pin GPIO_PIN_4
#define ICM_CS_GPIO_Port GPIOC
#define ICM_INT_Pin GPIO_PIN_5
#define ICM_INT_GPIO_Port GPIOC
#define ICM_INT_EXTI_IRQn EXTI9_5_IRQn
#define BNO_CS_Pin GPIO_PIN_12
#define BNO_CS_GPIO_Port GPIOB
#define DEBUG_TX_Pin GPIO_PIN_6
#define DEBUG_TX_GPIO_Port GPIOC
#define DEBUG_RX_Pin GPIO_PIN_7
#define DEBUG_RX_GPIO_Port GPIOC
#define BNO_INT_Pin GPIO_PIN_8
#define BNO_INT_GPIO_Port GPIOC
#define BNO_RST_Pin GPIO_PIN_9
#define BNO_RST_GPIO_Port GPIOC
#define BNO_WAK_Pin GPIO_PIN_8
#define BNO_WAK_GPIO_Port GPIOA
#define M8N_TX_Pin GPIO_PIN_10
#define M8N_TX_GPIO_Port GPIOC
#define M8N_RX_Pin GPIO_PIN_11
#define M8N_RX_GPIO_Port GPIOC
#define iBus_TX_Pin GPIO_PIN_12
#define iBus_TX_GPIO_Port GPIOC
#define iBus_RX_Pin GPIO_PIN_2
#define iBus_RX_GPIO_Port GPIOD
#define LPS_CS_Pin GPIO_PIN_6
#define LPS_CS_GPIO_Port GPIOB
#define LPS_INT_Pin GPIO_PIN_7
#define LPS_INT_GPIO_Port GPIOB


#endif /* SRC_BSP_BSP_H_ */
