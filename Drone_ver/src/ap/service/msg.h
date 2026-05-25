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




#endif /* SRC_AP_SERVICE_MSG_H_ */
