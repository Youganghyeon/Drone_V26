/*
 * led.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_LED_H_
#define SRC_COMMON_HW_INCLUDE_LED_H_

#include "hw_def.h"

#ifdef _USE_HW_LED


#define LED_MAX     HW_LED_MAX
#define DEF_LED_1   HW_DEF_LED_1
#define DEF_LED_2   HW_DEF_LED_2
#define DEF_LED_3   HW_DEF_LED_3


bool ledInit(void);
bool ledOn(uint8_t ch);
bool ledOff(uint8_t ch);
bool ledToggle(uint8_t ch);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_LED_H_ */
