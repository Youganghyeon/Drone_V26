/*
 * droneTm.h
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */

#ifndef HW_MODULE_DRONETM_DRONETM_H_
#define HW_MODULE_DRONETM_DRONETM_H_

#include "hw_def.h"

#define DEF_SW_MAX    HW_DEF_FSIA6B_SW_MAX_CH
#define DEF_SwA       HW_DEF_FSIA6B_SW_A
#define DEF_SwB       HW_DEF_FSIA6B_SW_B
#define DEF_SwC       HW_DEF_FSIA6B_SW_C
#define DEF_SwD       HW_DEF_FSIA6B_SW_D

typedef enum{
  Switch_high,
  Switch_IDLE,
  Switch_low
}Switch_state;



typedef struct
{
  uint16_t setRoll;
  uint16_t setPitch;
  uint16_t setthrottle;
  uint16_t setyaw;
  Switch_state  switch_ch[4];
  uint16_t knob_A;            // VrA -> 가변 저항 노브 A
  uint16_t knob_B;            // VrB -> 가변 저항 노브 B
  uint8_t  failsafe_status;    // 페일세이프 상태
  bool     is_connected;       // 연결 여부 (isOpen 변경)
  bool     is_Received;
}DroneTm_tbl;


void droneTmUpdate(void);
bool droneTmInit(void);
DroneTm_tbl* droneLinkData(void);
bool IsdroneTmInit(void);
#endif /* HW_MODULE_DRONETM_DRONETM_H_ */
