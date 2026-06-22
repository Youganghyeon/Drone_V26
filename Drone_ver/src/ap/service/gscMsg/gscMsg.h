/*
 * gcsMsg.h
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */

#ifndef AP_SERVICE_GCSMSG_GSCMSG_H_
#define AP_SERVICE_GCSMSG_GSCMSG_H_


#include "module.h"
#include "hw_def.h"

typedef enum{
  NONE    = 0x00,
  AHRS_MSG = 0x10,
  GPS_MSG,
  PID_Gain_MSG
}Service_MsgType;

#define DEF_MAX_SERVICE_MSG   1
#define DEF_SERVICE_MSG1      0

bool ServiceMsg_Init(void);
void EncodeMsg_AHRS(void);
void MsgEncode_PID_Gain(uint8_t id, float p, float i, float d);

//void MsgEncode_GPS(void);


#endif /* AP_SERVICE_GCSMSG_GSCMSG_H_ */
