/*
 * rohs.h
 *
 *  Created on: 2026. 5. 17.
 *      Author: yougang
 */

#ifndef SRC_HW_TELEMETRY_ROHS_ROHS_H_
#define SRC_HW_TELEMETRY_ROHS_ROHS_H_


#include "hw_def.h"

#ifdef _USE_HW_ROHS
typedef enum{
  ROHS_Header_H =0x00,
  ROHS_Header_L,
  ROHS_PayLoad,
  ROHS_Complete =0x13,
}ROHS_PacketField;

typedef struct{
  bool             isOpen;
  uint8_t          uartCh;

  uint8_t          rxBuf[20];
  uint8_t          rx_cpltFlag;
  ROHS_PacketField rx_status;

  uint8_t          txBuf[20];
  uint8_t          tx_cpltFlag;
  ROHS_PacketField tx_status;

}ROHS_tbl;

void ROHS_Init(void);
bool ROHS_Open(ROHS_tbl* p_rohs, uint8_t uart_ch);
bool ROHS_Read(ROHS_tbl* p_rohs);
bool ROHS_Write(ROHS_tbl* p_rohs, uint8_t* tx_data, uint32_t length);
bool ROHS_Parsing(ROHS_tbl* p_rohs, uint8_t cmd);
#endif /* SRC_HW_TELEMETRY_ROHS_ROHS_H_ */
#endif
