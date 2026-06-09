/*
 * gcs_msg.c
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */



#include "gsc_tm.h"
#include "sensor.h"
#include "dronetm.h"
#include "AT24C08.h"

gscTm_tbl       Service_msg;
DroneTm_tbl*     droneTm_data;
Sensor_tbl*      Sensor_data;

static bool isinit = false;

void ServiceMsg_Init(void)
{
  bool ret = false;
  bool s_ret = false;
  bool d_ret = false;
  if(isSensorInit()==true)
  {
    Sensor_data = sensorGetData();
    s_ret = true;
  }
  if(isdroneTmInit() == true)
  {
    droneTm_data = droneGetData();
    d_ret = true;
   }

  if(s_ret && d_ret)
  {
    isinit = true;
    ret =true;
  }
  return ret;
}

void EncodeMsg_AHRS(void)
{
  uint8_t txBuf[20];
  txBuf[0] = 0x46;
  txBuf[1] = 0x43;
  txBuf[2] = AHRS_MSG;

  txBuf[3] = (short)(Sensor_data->Roll*100);
  txBuf[4] = ((short)(Sensor_data->Roll*100))>>8;

  txBuf[5] = (short)(Sensor_data->Roll*100);
  txBuf[6] = ((short)(Sensor_data->Roll*100))>>8;

  txBuf[7] = (unsigned short)(Sensor_data->Yaw*100);
  txBuf[8] = ((unsigned short)(Sensor_data->Yaw*100))>>8;

  txBuf[9] =  (short)(Sensor_data->baroAltFilt*10);
  txBuf[10] = ((short)(Sensor_data->baroAltFilt*10))>>8;

  txBuf[11] = (short)((droneTm_data->setRoll - 1500)*0.1f*100);
  txBuf[12] = ((short)((droneTm_data->setRoll - 1500)*0.1f*100))>>8;

  txBuf[13] = (short)((droneTm_data->setPitch - 1500)*0.1f*100);
  txBuf[14] = ((short)((droneTm_data->setPitch - 1500)*0.1f*100))>>8;

  txBuf[15] = (unsigned short)((droneTm_data->setPitch -1000)*0.36f*100);
  txBuf[16] = ((unsigned short)((droneTm_data->setPitch - 1000)*0.36f*100))>>8;

  txBuf[17] = 0x00;
  txBuf[18] = 0x00;

  txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    txBuf[19] = txBuf[19] - txBuf[i];
  }
  gcsTmWrite(&txBuf[0], 20);
}

void MsgEncode_GPS(void)
{
  uint8_t txBuf[20];
  txBuf[0] = 0x46;
  txBuf[1] = 0x43;

  txBuf[2] = GPS_MSG;

  txBuf[3] = Sensor_data->lat;
  txBuf[4] = Sensor_data->lat>>8;
  txBuf[5] = Sensor_data->lat>>16;
  txBuf[6] = Sensor_data->lat>>24;

  txBuf[7]  = Sensor_data->lon;
  txBuf[8]  = Sensor_data->lon>>8;
  txBuf[9]  = Sensor_data->lon>>16;
  txBuf[10] = Sensor_data->lon>>24;

  txBuf[11] = 0;//(unsigned short)(batVolt*100);
  txBuf[12] = 0;//((unsigned short)(batVolt*100))>>8;

  txBuf[13] = 0;//iBus.SwA == 1000 ? 0 : 1;
  txBuf[14] = 0;//iBus.SwC == 1000 ? 0 : iBus.SwC == 1500 ? 1 : 2;

  txBuf[15] = 0;//iBus_isFailsafe(&iBus);

  txBuf[16] = 0x00;
  txBuf[17] = 0x00;
  txBuf[18] = 0x00;

  txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    txBuf[19] =txBuf[19] - txBuf[i];
  }
  gcsTmWrite(&txBuf[0], 20);
}

void MsgEncode_PID_Gain(uint8_t id, float p, float i, float d)
{
  uint8_t txBuf[20];
  txBuf[0] = 0x46;
  txBuf[1] = 0x43;

  txBuf[2] = id;

  //    memcpy(&telemetry_tx_buf[3], &p, 4);
  //    memcpy(&telemetry_tx_buf[7], &i, 4);
  //    memcpy(&telemetry_tx_buf[11], &d, 4);

  *(float*)&txBuf[3] = p;
  *(float*)&txBuf[7] = i;
  *(float*)&txBuf[11]=d;

  txBuf[15] = 0x00;
  txBuf[16] = 0x00;
  txBuf[17] = 0x00;
  txBuf[18] = 0x00;

  txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    txBuf[19] =  txBuf[19] -  txBuf[i];
  }
  gcsTmWrite(&txBuf[0], 20);
}
