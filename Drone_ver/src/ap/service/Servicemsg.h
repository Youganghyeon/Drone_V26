/*
 * msg.h
 *
 *  Created on: 2026. 5. 25.
 *      Author: yougang
 */

#ifndef SRC_AP_SERVICE_MSG_H_
#define SRC_AP_SERVICE_MSG_H_

#include "hw_def.h"


typedef enum{
  NONE    = 0x00,
  AHRS_MSG,
  GPS_MSG,
  PID_Gain_MSG
}Service_MsgType;

#define DEF_MAX_SERVICE_MSG   1
#define DEF_SERVICE_MSG1      0

void ServiceMsg_Init(void);
bool ServiceMsg_Open(void);
void EncodeMsg_AHRS(ROHS_tbl* p_rohs, BNO080_tbl* p_bno, LPS22HH_tbl_t* p_lps);
void MsgEncode_GPS(ROHS_tbl* p_rohs, M8N_tbl* p_m8n);
void MsgEncode_PID_Gain(ROHS_tbl* p_rohs, uint8_t id, float p, float i, float d);

#endif /* SRC_AP_SERVICE_MSG_H_ */
