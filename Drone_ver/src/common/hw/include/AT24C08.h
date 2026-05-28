/*
 * AT24C08.h
 *
 *  Created on: 2026. 5. 20.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_AT24C08_H_
#define SRC_COMMON_HW_INCLUDE_AT24C08_H_

#include "hw_def.h"

typedef enum{
  PID_Roll_in = 0x00,
  PID_Roll_out,
  PID_Pitch_in,
  PID_Pitch_out,
  PID_Yaw_in,
  PID_Yaw_out,
  PID_All,
}PID_Angle;


bool AT24C08_isInit(void);
bool AT24C08_isOpen(void);
bool AT24C08_Open(void);
void AT24C08_Init(void);
void EP_PIDGain_Write(uint8_t id, float PGain, float IGain, float DGain);
bool EP_PIDGain_Read(PID_Angle id, float* PGain, float* IGain, float* DGain);
#endif /* SRC_COMMON_HW_INCLUDE_AT24C08_H_ */
