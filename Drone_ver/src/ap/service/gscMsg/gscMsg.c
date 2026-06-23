/*
 * gcs_msg.c
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */



#include "gcstm.h"
#include "sensor.h"
#include "dronetm.h"
#include "AT24C08.h"
#include "gscMsg.h"

static void Encode_Msg_PID_TxRx(void);

static gscTm_tbl*      gcs_data;
static DroneTm_tbl*    droneTm_data;
static Sensor_tbl*     Sensor_data;
static bool isinit   = false;
static uint8_t txBuf[20];


bool ServiceMsg_Init(void)
{
  bool ret = false;
  bool s_ret = false;
  bool d_ret = false;
  bool g_ret = false;
  if(IsgscTmInit()==true)
  {
    gcs_data = gcsTmGetData();
    g_ret = true;
    gcsTmRegister_RxFunc(gcsTmRxcplt_Func1, Encode_Msg_PID_TxRx);
  }
  if(isSensorInit()==true)
  {
    Sensor_data = sensorGetData();
    s_ret = true;
  }
  if(IsdroneTmInit() == true)
  {
    droneTm_data = droneLinkData();
    d_ret = true;
   }

  if(s_ret && d_ret&& g_ret)
  {
    isinit = true;
    ret =true;
  }
  return ret;
}

static void Encode_Msg_PID_TxRx(void)
{
  if(droneTm_data->switch_ch[DEF_SwA] == Switch_low)
  {
    float kp;
    float ki;
    float kd;
    uint8_t *p_data = gcs_data->gscTm_rx;
    switch(p_data[2])
    {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
        kp = *(float*)&p_data[3];
        ki = *(float*)&p_data[7];
        kd = *(float*)&p_data[11];
        EP_PIDGain_Write(p_data[2], kp, ki, kd);
        EP_PIDGain_Read(p_data[2], &kp, &ki, &kd);
        MsgEncode_PID_Gain(p_data[2], kp, ki, kd);
        gcsTmWrite(&txBuf[0], 20);
        break;

    }
  }
}



void EncodeMsg_AHRS(void)
{

  txBuf[0] = 0x46;
  txBuf[1] = 0x43;
  txBuf[2] = 0x10;

  txBuf[3] = (short)(Sensor_data->roll*100.0f);
  txBuf[4] = ((short)(Sensor_data->roll*100.0f))>>8;

  txBuf[5] = (short)(Sensor_data->pitch*100.0f);
  txBuf[6] = ((short)(Sensor_data->pitch*100.0f))>>8;

  txBuf[7] = (unsigned short)(Sensor_data->yaw*100.0f);
  txBuf[8] = ((unsigned short)(Sensor_data->yaw*100.0f))>>8;

  txBuf[9] =  (short)(Sensor_data->alt_filt*10.0f);
  txBuf[10] = ((short)(Sensor_data->alt_filt*10.0f))>>8;

  txBuf[11] = (short)((droneTm_data->setRoll - 1500)*0.1f*100);
  txBuf[12] = ((short)((droneTm_data->setRoll - 1500)*0.1f*100))>>8;

  txBuf[13] = (short)((droneTm_data->setPitch - 1500)*0.1f*100);
  txBuf[14] = ((short)((droneTm_data->setPitch - 1500)*0.1f*100))>>8;

  txBuf[15] = (unsigned short)((droneTm_data->setyaw -1000)*0.36f*100);
  txBuf[16] = ((unsigned short)((droneTm_data->setyaw - 1000)*0.36f*100))>>8;

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

  txBuf[13] = droneTm_data->switch_ch[DEF_SwA] == 1000 ? 0 : 1;
  txBuf[14] = droneTm_data->switch_ch[DEF_SwC] == 1000 ? 0 : droneTm_data->switch_ch[DEF_SwC] == 1500 ? 1 : 2;

  txBuf[15] = droneTm_data->failsafe_status;

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
