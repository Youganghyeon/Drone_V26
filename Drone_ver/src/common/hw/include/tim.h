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

#define TIM_TYPE_PWM_FLAG        (0x00)
#define TIM_TYPE_TIMER_FLAG      (0x80)

#define    TIM_IS_TIMER(ch)     (ch & 0x80)
#define    TIM_GET_INDEX(ch)    (ch & 0x7F)


#define    DEF_TIM3_CH4         (TIM_TYPE_PWM_FLAG   | HW_DEF_TIM3_CH4)
#define    DEF_TIM5_CH1         (TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH1)
#define    DEF_TIM5_CH2         (TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH2)
#define    DEF_TIM5_CH3         (TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH3)
#define    DEF_TIM5_CH4         (TIM_TYPE_PWM_FLAG   | HW_DEF_TIM5_CH4)

#define    DEF_TIM7             (TIM_TYPE_TIMER_FLAG | HW_DEF_TIM7)





void timInit(void);
bool timOpen(uint8_t ch);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
bool pwmStart(uint8_t ch);
bool timPsc(uint8_t ch, uint32_t psc);
bool pwmStop(uint8_t ch);
bool timDeinit(uint8_t ch);
bool Is1msFlag(uint8_t ch);
bool Is20msFlag(uint8_t ch);
bool clear1msFlag(uint8_t ch);
bool clear20msFlag(uint8_t ch);
#endif


#endif /* SRC_COMMON_HW_INCLUDE_TIM_H_ */
