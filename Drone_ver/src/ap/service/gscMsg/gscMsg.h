/*
 * gcsMsg.h
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */

#ifndef AP_SERVICE_GCSMSG_GSCMSG_H_
#define AP_SERVICE_GCSMSG_GSCMSG_H_


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
void EncodeMsg_AHRS(void);
void MsgEncode_GPS(void);
void MsgEncode_PID_Gain(uint8_t id, float p, float i, float d);


#endif /* AP_SERVICE_GCSMSG_GSCMSG_H_ */
