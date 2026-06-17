/*
 * esc.h
 *
 *  Created on: 2026. 6. 2.
 *      Author: yougang
 */

#ifndef SRC_HW_ESC_ESC_H_
#define SRC_HW_ESC_ESC_H_

#include "hw_def.h"
#ifdef _USE_HW_ESC


#define ESC_IDLE_SPEED    10500
#define ESC_MAX_SPEED     42000

#define ESC_MAX_CH        4

#define DEF_ESC1          DEF_TIM5_CH1
#define DEF_ESC2          DEF_TIM5_CH2
#define DEF_ESC3          DEF_TIM5_CH3
#define DEF_ESC4          DEF_TIM5_CH4
void escInit(void);
bool escOpen(uint8_t ch);
bool escCalibration(void);
bool escOutput(uint8_t ch, uint32_t speed);
#endif
#endif /* SRC_HW_ESC_ESC_H_ */
