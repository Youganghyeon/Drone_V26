/*
 * msg.c
 *
 *  Created on: 2026. 5. 25.
 *      Author: yougang
 */


#include "Servicemsg.h"
#include "FS-iA6B/fs-ia6b.h"
#include "M8N/m8n.h"
#include "ICM20602/ICM20602.h"
#include "LPS22HH/LPS22HH.h"
#include "BNO080/BNO080.h"
#include "ROHS/rohs.h"
#include "AT24C08.h"


typedef struct{
  bool             isOpen;
  uint8_t          channel;
  Service_MsgType  MsgType;
  ROHS_tbl*        Rohs;
}Service_Msg_tbl;


Service_Msg_tbl Service_msg;

void ServiceMsg_Init(void)
{
}

bool ServiceMsg_Open(void)
{
  bool ret=true;
  Service_msg.isOpen=true;
  return ret;
}

void EncodeMsg_AHRS(ROHS_tbl* p_rohs, BNO080_tbl* p_bno, LPS22HH_tbl_t* p_lps)
{
  BNO080_Angle_tbl* p_bno_angle = &p_bno->BNO080_Angle;
  p_rohs->txBuf[0] = 0x46;
  p_rohs->txBuf[1] = 0x43;
  p_rohs->txBuf[2] = AHRS_MSG;

  p_rohs->txBuf[3] = (short)(p_bno_angle->Roll*100);
  p_rohs->txBuf[4] = ((short)(p_bno_angle->Roll*100))>>8;

  p_rohs->txBuf[5] = (short)(p_bno_angle->Roll*100);
  p_rohs->txBuf[6] = ((short)(p_bno_angle->Roll*100))>>8;

  p_rohs->txBuf[7] = (unsigned short)(p_bno_angle->Yaw*100);
  p_rohs->txBuf[8] = ((unsigned short)(p_bno_angle->Yaw*100))>>8;

  p_rohs->txBuf[9] =  (short)(p_lps->baroAltFilt*10);
  p_rohs->txBuf[10] = ((short)(p_lps->baroAltFilt*10))>>8;

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

void MsgEncode_GPS(ROHS_tbl* p_rohs, M8N_tbl* p_m8n)
{
  M8N_UBX_NAV_POSLLH* p_poslh = &p_m8n->posllh;
  p_rohs->txBuf[0] = 0x46;
  p_rohs->txBuf[1] = 0x43;

  p_rohs->txBuf[2] = GPS_MSG;

  p_rohs->txBuf[3] = p_poslh->lat;
  p_rohs->txBuf[4] = p_poslh->lat>>8;
  p_rohs->txBuf[5] = p_poslh->lat>>16;
  p_rohs->txBuf[6] = p_poslh->lat>>24;

  p_rohs->txBuf[7] = p_poslh->lon;
  p_rohs->txBuf[8] = p_poslh->lon>>8;
  p_rohs->txBuf[9] = p_poslh->lon>>16;
  p_rohs->txBuf[10] = p_poslh->lon>>24;

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
