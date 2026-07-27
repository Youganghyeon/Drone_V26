// sensor.c
#include "sensor.h"
#include "ICM20602.h"
#include "BNO080.h"
#include "LPS22HH.h"
#include "M8N.h"
#ifdef _USE_HW_SENSOR
static LPS22HH_tbl_t  LPS22HH;
static ICM20602_tbl_t ICM20602;
static BNO080_tbl     BNO080;
static M8N_tbl        M8N;

static Sensor_tbl sensor_data;
static bool is_init = false;

#define APPLY_SIGN(val)   ((val) * (-1.0f))

bool sensorInit(void)
{
  bool ret = true;
  M8N_Open();
  if(LPS22HH_Open(&LPS22HH) != true)
  {
    ret = false;
  }
  if(ICM20602_Open(&ICM20602) != true)
  {
    ret = false;
  }
  if(BNO080_Open(&BNO080) != true)
  {
    ret = false;
  }
  BNO080_enableRotationVector(&BNO080, 2500);
  delay(400);
  is_init = ret;
  return ret;
}

bool isSensorInit(void)
{
  return is_init;
}


void sensorCalibration(Calibration_Sensor_Type isCalibration_OK)
{
  if((isCalibration_OK == BNO080_Cali_Step1) || (isCalibration_OK == BNO080_Cali_Step2))
  {
    BNO080_Calibration(&BNO080, isCalibration_OK);
  }
  if(isCalibration_OK == ICM20602_Cali)
  {
    short gyro_x_offset = 1, gyro_y_offset = -23, gyro_z_offset = -3;
    ICM20602_Calibration(gyro_x_offset, gyro_y_offset, gyro_z_offset);
  }

}

void sensorUpdate(void)
{
  if(is_init != true) return;

  if(LPS22HH_GetInfo(&LPS22HH, LPS22HH_GetPress)==true)
  {
    LPS22HH_GetAlt(&LPS22HH, TEMP_CORRECT);
    LPS22HH_GetAltFilt(&LPS22HH);
    sensor_data.alt         = LPS22HH.baroAlt;
    sensor_data.alt_filt    = LPS22HH.baroAltFilt;
    sensor_data.press_raw   = LPS22HH.pressure_raw;
    sensor_data.temp_raw    = LPS22HH.temperature_raw;

  }
  if(ICM20602_GetInfo(&ICM20602, AxisGyroRaw)==true)
  {
    sensor_data.gyro_x      = APPLY_SIGN(ICM20602.gyro_x);
    sensor_data.gyro_y      = ICM20602.gyro_y;
    sensor_data.gyro_z      = APPLY_SIGN(ICM20602.gyro_z);

  }
  if(BNO080_ReadInfo(&BNO080)==true)
  {
    sensor_data.roll        = APPLY_SIGN(BNO080.BNO080_Angle.Roll);
    sensor_data.pitch       = APPLY_SIGN(BNO080.BNO080_Angle.Pitch);
    sensor_data.yaw         = BNO080.BNO080_Angle.Yaw;
  }

  /*M8N_ReceivePacket(&M8N);
  if(M8N.RxBuf.m8n_cplt_flag == 1)
  {
    M8N.RxBuf.m8n_cplt_flag = 0;
    if(M8N_Checksum_Check(&M8N.RxBuf.buf[0], 36) == 1)
    {
      M8N_Parsing(&M8N.RxBuf.buf[0], &M8N.posllh);
      sensor_data.lat         = M8N.posllh.lat;
      sensor_data.lon         = M8N.posllh.lon;
      sensor_data.height      = M8N.posllh.height;
    }
  }*/
}

Sensor_tbl* sensorGetData(void)
{
  return &sensor_data;
}
#endif
