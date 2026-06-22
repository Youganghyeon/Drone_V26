/*
 * failSafe.h
 *
 *  Created on: 2026. 6. 10.
 *      Author: yougang
 */

#ifndef AP_SERVICE_FAILSAFE_FAILSAFE_H_
#define AP_SERVICE_FAILSAFE_FAILSAFE_H_

#include "module.h"
#include "hw_def.h"

#define DEF_SENSOR1       0
#define DEF_SENSOR2       1
#define DEF_SENSOR3       2
bool FailSafe_SensorError(uint8_t sensor_ch);
bool FailSafe_RCLost(void);
bool FailSafe_LowBattery(void);
bool FailSafe_Indicate(uint8_t flag);

#endif /* AP_SERVICE_FAILSAFE_FAILSAFE_H_ */
