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

#define TIM_MAX_CH         HW_TIM_MAX_CH
#define DEF_TIM3_CH4       HW_DEF_TIM3_CH4
#define DEF_TIM7_TIMER     HW_DEF_TIM7_TIMER


void timInit(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
bool pwmStart(uint8_t ch);
bool pwmPsc(uint8_t ch, uint32_t psc);
bool pwmStop(uint8_t ch);
bool timDeinit(uint32_t ch);
bool Is1msFlag(uint8_t ch);
bool clear1msFlag(uint8_t ch);
#endif


#endif /* SRC_COMMON_HW_INCLUDE_TIM_H_ */
