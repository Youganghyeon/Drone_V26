/*
 * tim.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_TIM_H_
#define SRC_COMMON_HW_INCLUDE_TIM_H_

#include "hw_def.h"

#ifdef _USE_HW_TIM

#define PWM3_CH4           HW_PWM3_CH4

bool timInit(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
bool pwmStart(uint8_t ch);
bool pwmPsc(uint8_t ch, uint32_t psc);
bool pwmStop(uint8_t ch);
bool timDeinit(void);
#endif


#endif /* SRC_COMMON_HW_INCLUDE_TIM_H_ */
