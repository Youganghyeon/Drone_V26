/*
 * msg.c
 *
 *  Created on: 2026. 5. 25.
 *      Author: yougang
 */


#include "Service_Telemetery.h"
#include "sensor.h"
#include "FS-iA6B/fs-ia6b.h"
#include "ROHS/rohs.h"
#include "AT24C08.h"


typedef struct{
  bool             isOpen;
  uint8_t          channel;
  Service_MsgType  MsgType;
  ROHS_tbl*        Rohs;
}Service_Msg_tbl;

Service_Msg_tbl Service_msg;
Sensor_tbl      Sensor_data;
void ServiceMsg_Init(void)
{
  if(isSensorInit()==false)
  {
    return false;
  }
  Sensor_data = sensorGetData();
}

bool ServiceMsg_Open(void)
{
  bool ret=true;
  Service_msg.isOpen=true;
  return ret;
}

void EncodeMsg_AHRS(ROHS_tbl* p_rohs)
{
  BNO080_Angle_tbl* p_bno_angle = &p_bno->BNO080_Angle;
  p_rohs->txBuf[0] = 0x46;
  p_rohs->txBuf[1] = 0x43;
  p_rohs->txBuf[2] = AHRS_MSG;

  p_rohs->txBuf[3] = (short)(Sensor_data->Roll*100);
  p_rohs->txBuf[4] = ((short)(Sensor_data->Roll*100))>>8;

  p_rohs->txBuf[5] = (short)(Sensor_data->Roll*100);
  p_rohs->txBuf[6] = ((short)(Sensor_data->Roll*100))>>8;

  p_rohs->txBuf[7] = (unsigned short)(Sensor_data->Yaw*100);
  p_rohs->txBuf[8] = ((unsigned short)(Sensor_data->Yaw*100))>>8;

  p_rohs->txBuf[9] =  (short)(Sensor_data->baroAltFilt*10);
  p_rohs->txBuf[10] = ((short)(Sensor_data->baroAltFilt*10))>>8;

  p_rohs->txBuf[11] = 0;//(short)((iBus.RH-1500)*0.1f*100);
  p_rohs->txBuf[12] = 0;//((short)((iBus.RH-1500)*0.1f*100))>>8;

  p_rohs->txBuf[13] = 0;//(short)((iBus.RV-1500)*0.1f*100);
  p_rohs->txBuf[14] = 0;//((short)((iBus.RV-1500)*0.1f*100))>>8;

  p_rohs->txBuf[15] = 0;//(unsigned short)((iBus.LH-1000)*0.36f*100);
  p_rohs->txBuf[16] = 0;//((unsigned short)((iBus.LH-1000)*0.36f*100))>>8;

  p_rohs->txBuf[17] = 0x00;
  p_rohs->txBuf[18] = 0x00;

  p_rohs->txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    p_rohs->txBuf[19] = p_rohs->txBuf[19] - p_rohs->txBuf[i];
  }
}

void MsgEncode_GPS(void)
{
  M8N_UBX_NAV_POSLLH* p_poslh = &p_m8n->posllh;
  p_rohs->txBuf[0] = 0x46;
  p_rohs->txBuf[1] = 0x43;

  p_rohs->txBuf[2] = GPS_MSG;

  p_rohs->txBuf[3] = Sensor_data->lat;
  p_rohs->txBuf[4] = Sensor_data->lat>>8;
  p_rohs->txBuf[5] = Sensor_data->lat>>16;
  p_rohs->txBuf[6] = Sensor_data->lat>>24;

  p_rohs->txBuf[7]  = Sensor_data->lon;
  p_rohs->txBuf[8]  = Sensor_data->lon>>8;
  p_rohs->txBuf[9]  = Sensor_data->lon>>16;
  p_rohs->txBuf[10] = Sensor_data->lon>>24;

  p_rohs->txBuf[11] = 0;//(unsigned short)(batVolt*100);
  p_rohs->txBuf[12] = 0;//((unsigned short)(batVolt*100))>>8;

  p_rohs->txBuf[13] = 0;//iBus.SwA == 1000 ? 0 : 1;
  p_rohs->txBuf[14] = 0;//iBus.SwC == 1000 ? 0 : iBus.SwC == 1500 ? 1 : 2;

  p_rohs->txBuf[15] = 0;//iBus_isFailsafe(&iBus);

  p_rohs->txBuf[16] = 0x00;
  p_rohs->txBuf[17] = 0x00;
  p_rohs->txBuf[18] = 0x00;

  p_rohs->txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    p_rohs->txBuf[19] = p_rohs->txBuf[19] - p_rohs->txBuf[i];
  }
}

void MsgEncode_PID_Gain(ROHS_tbl* p_rohs, uint8_t id, float p, float i, float d)
{
  p_rohs->txBuf[0] = 0x46;
  p_rohs->txBuf[1] = 0x43;

  p_rohs->txBuf[2] = id;

  //    memcpy(&telemetry_tx_buf[3], &p, 4);
  //    memcpy(&telemetry_tx_buf[7], &i, 4);
  //    memcpy(&telemetry_tx_buf[11], &d, 4);

  *(float*)& p_rohs->txBuf[3] = p;
  *(float*)& p_rohs->txBuf[7] = i;
  *(float*)& p_rohs->txBuf[11]=d;

  p_rohs->txBuf[15] = 0x00;
  p_rohs->txBuf[16] = 0x00;
  p_rohs->txBuf[17] = 0x00;
  p_rohs->txBuf[18] = 0x00;

  p_rohs->txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    p_rohs->txBuf[19] =  p_rohs->txBuf[19] -  p_rohs->txBuf[i];
  }
}
