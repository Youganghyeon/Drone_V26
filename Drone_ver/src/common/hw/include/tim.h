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

/*
#define DEF_TIM7_TIMER     HW_DEF_TIM7_TIMER


#define DEF_TIM3_CH4       HW_DEF_TIM3_CH4
#define DEF_TIM5_CH1       HW_DEF_TIM5_CH1
#define DEF_TIM5_CH2       HW_DEF_TIM5_CH2
#define DEF_TIM5_CH3       HW_DEF_TIM5_CH2
#define DEF_TIM5_CH4       HW_DEF_TIM5_CH4

*/

#define TIM_MAX_CH               HW_TIM_MAX_CH
#define PWM_MAX_CH               HW_PWM_MAX_CH
#define TIMER_MAX_CH             HW_TIMER_MAX_CH

#define TIM_TYPE_PWM_FLAG        HW_TIM_TYPE_PWM_FLAG
#define TIM_TYPE_TIMER_FLAG      HW_TIM_TYPE_TIMER_FLAG

#define TIM_IS_TIMER(ch)         HW_TIM_IS_TIMER(ch)
#define TIM_GET_INDEX(ch)        HW_TIM_GET_INDEX(ch)


#define TIM3_CH4                 DEF_TIM3_CH4
#define TIM5_CH1                 DEF_TIM5_CH1
#define TIM5_CH2                 DEF_TIM5_CH2
#define TIM5_CH3                 DEF_TIM5_CH3
#define TIM5_CH4                 DEF_TIM5_CH4

#define TIM7_TIMER               DEF_TIM7





void timInit(void);
bool timOpen(uint8_t ch);
bool istimOpen(uint8_t ch);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
bool pwmStart(uint8_t ch);
bool pwmChange(uint8_t ch, uint32_t ccr);
bool timPsc(uint8_t ch, uint32_t psc);
bool pwmStop(uint8_t ch);
bool timDeinit(uint8_t ch);
bool Is1msFlag(uint8_t ch);
bool Is20msFlag(uint8_t ch);
bool clear1msFlag(uint8_t ch);
bool clear20msFlag(uint8_t ch);
#endif


#endif /* SRC_COMMON_HW_INCLUDE_TIM_H_ */
