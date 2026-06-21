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

typedef enum{
  BNO080_Cali_Step1 = 0x01,
  BNO080_Cali_Step2,
  ICM20602_Cali
}Calibration_Sensor_Type;

#define        SENOSR_MAX_CH      HW_SENSOR_MAX_CH
#define        DEF_SENSOR1       _DEF_HW_ICM20602
#define        DEF_SENSOR2       _DEF_HW_BNO080
#define        DEF_SENSOR3       _DEF_HW_LPS22HH


bool           sensorInit(void);
void           sensorUpdate(void);
Sensor_tbl*    sensorGetData(void);
bool           isSensorInit(void);
void           sensorCalibration(Calibration_Sensor_Type isCalibration_OK);

#endif /* SRC_HW_SENSOR_SENSOR_H_ */
