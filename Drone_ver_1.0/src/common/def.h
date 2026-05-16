/*
 * def.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stdarg.h"
#include "stdbool.h"
#include "math.h"

#define LED_1_Pin GPIO_PIN_0
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_1
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin GPIO_PIN_2
#define LED_3_GPIO_Port GPIOC
#define ICM_CS_Pin GPIO_PIN_4
#define ICM_CS_GPIO_Port GPIOC
#define ICM_INT_Pin GPIO_PIN_5
#define ICM_INT_GPIO_Port GPIOC
#define BNO_CS_Pin GPIO_PIN_12
#define BNO_CS_GPIO_Port GPIOB
#define BNO_INT_Pin GPIO_PIN_8
#define BNO_INT_GPIO_Port GPIOC
#define BNO_RST_Pin GPIO_PIN_9
#define BNO_RST_GPIO_Port GPIOC
#define BNO_WAK_Pin GPIO_PIN_8
#define BNO_WAK_GPIO_Port GPIOA
#define LPS_CS_Pin GPIO_PIN_6
#define LPS_CS_GPIO_Port GPIOB
#define LPS_INT_Pin GPIO_PIN_7
#define LPS_INT_GPIO_Port GPIOB


#endif /* SRC_COMMON_DEF_H_ */
