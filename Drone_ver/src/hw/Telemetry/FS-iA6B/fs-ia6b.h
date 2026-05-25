/*
 * fs-i6ab.h
 *
 *  Created on: 2026. 5. 25.
 *      Author: yougang
 */

#ifndef SRC_HW_TELEMETRY_FS_IA6B_FS_IA6B_H_
#define SRC_HW_TELEMETRY_FS_IA6B_FS_IA6B_H_

#include "hw_def.h"


typedef struct
{
  uint16_t RH; //Right Horizontal
  uint16_t RV; //Right Vertical
  uint16_t LV; //Left Vertical
  uint16_t LH; //Left Horizontal
  uint16_t SwA;
  uint16_t SwB;
  uint16_t SwC;
  uint16_t SwD;
  uint16_t VrA;
  uint16_t VrB;

  uint8_t FailSafe;
  bool    isOpen;
}FSiA6B_tbl;


void FSIA6B_Init(void);
bool FSIA6B_Open(FSiA6B_tbl* p_iBus);
bool iBus_Check_CHKSUM(uint8_t* data, uint8_t len);

#endif /* SRC_HW_TELEMETRY_FS_IA6B_FS_IA6B_H_ */
