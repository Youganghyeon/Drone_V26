/*
 * cmd.h
 *
 *  Created on: 2026. 7. 27.
 *      Author: yougang
 */

#ifndef COMMON_HW_INCLUDE_CMD_H_
#define COMMON_HW_INCLUDE_CMD_H_

#include "hw_def.h"

#ifdef _USE_HW_CMD
#define CMD_MAX_DATA_LENGTH       HW_CMD_MAX_DATA_LENGTH

#define CMD_OK        1
#define CMD_FAIL      2

#define DEF_CMD_STX   0x50
#define DEF_CMD_EOF   0x80

#define CMD_PC_TO_MCU 0x00
#define CMD_MCU_TO_PC 0x01
#define cmdTimeout    500

typedef enum
{
  CMD_STX,
  CMD_HEADER,
  CMD_SEQ_L,
  CMD_SEQ_H,
  CMD_DIR,
  CMD_ERROR,
  CMD_LENGTH_L,
  CMD_LENGTH_H,
  CMD_DATA,
  CMD_CRC,
  CMD_EOF,
}cmd_state_tbl;


typedef struct
{
  uint8_t Header;
  uint16_t Seq;
  uint8_t Dir;
  uint8_t Error;
  uint16_t Length;
  uint8_t Buf[CMD_MAX_DATA_LENGTH];
  uint8_t* Data;
  uint8_t Crc;
  uint8_t Eof;
  uint16_t index;
  uint8_t Crc_prev;
  uint16_t SeqExpect;
  bool     Valid;
}cmd_packet_tbl;

typedef struct
{
  bool isOpen;
  uint8_t ch;
  uint8_t state;
  uint8_t error;
  cmd_packet_tbl rx_packet;
  cmd_packet_tbl tx_packet;
}cmd_tbl;

void cmdInit(void);
bool cmdReceivePacket(cmd_tbl* p_cmd);
void cmdOpen(cmd_tbl* p_cmd, uint8_t ch, uint32_t baud);
void cmdSend(cmd_tbl* p_cmd, uint8_t cmdHeader, uint8_t* p_data, uint16_t length);
bool cmdSendResp_Wait(cmd_tbl* p_cmd, uint8_t cmdHeader, uint8_t* p_data, uint16_t length);
void cmdRespond(cmd_tbl* p_cmd, uint8_t cmdHeader, uint8_t error, uint8_t* p_data, uint16_t length);

#endif

#endif /* COMMON_HW_INCLUDE_CMD_H_ */
