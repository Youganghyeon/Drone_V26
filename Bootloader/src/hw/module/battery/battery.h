/*
 * battery.h
 *
 *  Created on: 2026. 6. 28.
 *      Author: yougang
 */

#ifndef HW_MODULE_BATTERY_BATTERY_H_
#define HW_MODULE_BATTERY_BATTERY_H_

#include "hw_def.h"
#include "module_def.h"

void batInit(void);
bool batStart(void);
float batCheck(void);
bool isBatInit(void);
#endif /* HW_MODULE_BATTERY_BATTERY_H_ */
