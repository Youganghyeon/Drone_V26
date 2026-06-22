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
bool Alarm_SensorError(uint8_t sensor_ch);
bool Alarm_RCLost(void);
bool Alarm_LowBattery(void);
bool Alarm_Indicate(uint8_t flag);

#endif /* AP_SERVICE_FAILSAFE_FAILSAFE_H_ */
