/*
 * sensor.h
 *
 *  Created on: 2026. 5. 16.
 *      Author: yougang
 */

#ifndef SRC_HW_SENSOR_SENSOR_H_
#define SRC_HW_SENSOR_SENSOR_H_

#include "hw_def.h"

typedef struct{
  float roll;
  float pitch;
  float yaw;
  float gyro_x;
  float gyro_y;
  float gyro_z;
  float alt;
  float alt_filt;
  float press_raw;
  float temp_raw;
  int32_t lat;
  int32_t lon;
  int32_t height;
}Sensor_tbl;

bool           sensorInit(void);
void           sensorUpdate(void);
Sensor_tbl*    sensorGetData(void);
bool           isSensorInit(void);
#endif /* SRC_HW_SENSOR_SENSOR_H_ */
