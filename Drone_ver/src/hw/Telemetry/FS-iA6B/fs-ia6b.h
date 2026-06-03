/*
 * fs-i6ab.h
 *
 *  Created on: 2026. 5. 25.
 *      Author: yougang
 */

#ifndef SRC_HW_TELEMETRY_FS_IA6B_FS_IA6B_H_
#define SRC_HW_TELEMETRY_FS_IA6B_FS_IA6B_H_

#include "hw_def.h"
#ifdef _USE_HW_FSIA6B
#define     SW_A      0
#define     SW_B      1
#define     SW_C      2
#define     SW_D      3

typedef struct
{
  uint16_t RH; //Right Horizontal
  uint16_t RV; //Right Vertical
  uint16_t LV; //Left Vertical
  uint16_t LH; //Left Horizontal
  uint16_t SW[4];
  uint16_t VrA;
  uint16_t VrB;

  uint8_t rxBuf[32];
  uint16_t checksum;
  bool    ibus_rx_cplt_flag;

  uint8_t FailSafe;
  bool    isOpen;
}FSiA6B_tbl;


void FSIA6B_Init(void);
bool FSIA6B_Open(FSiA6B_tbl* p_iBus);
void FSIA6B_Parsing(FSiA6B_tbl* iBus);
bool FSIA6B_Check_checkSum(FSiA6B_tbl* iBus, uint8_t len);
bool FSIA6B_RecivePacket(FSiA6B_tbl* iBus);
bool FSIA6B_isFailsafe(FSiA6B_tbl* iBus);
#endif /* SRC_HW_TELEMETRY_FS_IA6B_FS_IA6B_H_ */
#endif
