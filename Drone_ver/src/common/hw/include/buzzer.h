/*
 * bozzer.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_BUZZER_H_
#define SRC_COMMON_HW_INCLUDE_BUZZER_H_

#include "hw_def.h"

#ifdef _USE_HW_BUZZER


#define DEF_BUZZER    PWM3_CH4

bool    buzInit(void);
bool    buzSetPitch(uint32_t pitch);
bool    buzDeinit(void);

#endif
#endif /* SRC_COMMON_HW_INCLUDE_BUZZER_H_ */
