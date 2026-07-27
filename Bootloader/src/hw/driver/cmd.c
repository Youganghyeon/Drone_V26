/*
 * cmd.c
 *
 *  Created on: 2026. 7. 27.
 *      Author: yougang
 */


#include "cmd.h"
#include "uart.h"

#ifdef _USE_HW_CMD

void cmdInit(void)
{
}

void cmdOpen(cmd_tbl* p_cmd, uint8_t ch, uint32_t baud)
{
  bool ret = false;
  if(IsUartOpen(ch) == true)
  {
    ret= true;
  }
  else
  {
    if(uartOpen(ch, baud)==true)
    {
      ret = true;
    }
  }
  if(ret == true)
  {
    p_cmd->isOpen = true;
    p_cmd->ch = ch;
    p_cmd->state = CMD_STX;
    p_cmd->rx_packet.SeqExpect = 0;
    p_cmd->rx_packet.Data=&p_cmd->rx_packet.Buf[CMD_STX];
    p_cmd->tx_packet.Data=&p_cmd->tx_packet.Buf[CMD_STX];
  }
}

bool cmdReceivePacket(cmd_tbl* p_cmd)
{
  cmd_packet_tbl* p_rx_packet = &p_cmd->rx_packet;
  bool ret= false;
  if(p_cmd->isOpen == true)
  {
    if(uartAvailable(p_cmd->ch))
    {
      uint8_t rx_buf;
      rx_buf= uartRead(p_cmd->ch);
      bool eof_ret = false;
      bool crc_ret = false;
      bool seq_ret = false;
      switch(p_cmd->state)
      {
        case CMD_STX:
          if(rx_buf == DEF_CMD_STX)
          {
            p_cmd->state     = CMD_HEADER;
            p_rx_packet->Crc = 0;
            p_rx_packet->index=0;
          }
          break;
        case CMD_HEADER:
          p_rx_packet->Header =  rx_buf;
          p_rx_packet->Crc    += rx_buf;
          p_cmd->state        = CMD_SEQ_L;
          break;
        case CMD_SEQ_L:
          p_rx_packet->Seq    =  rx_buf;
          p_rx_packet->Crc    += rx_buf;
          p_cmd->state        =  CMD_SEQ_H;
          break;
        case CMD_SEQ_H:
          p_rx_packet->Seq    |= (rx_buf<<8);
          p_rx_packet->Crc    += rx_buf;
          p_cmd->state        =  CMD_DIR;
          break;
        case CMD_DIR:
          p_rx_packet->Dir    = rx_buf;
          p_rx_packet->Crc    += rx_buf;
          p_cmd->state        = CMD_ERROR;
          break;
        case CMD_ERROR:
          p_rx_packet->Error  = rx_buf;
          p_rx_packet->Crc    += rx_buf;
          p_cmd->state        = CMD_LENGTH_L;
          break;
        case CMD_LENGTH_L:
          p_rx_packet->Length = rx_buf;
          p_rx_packet->Crc    += rx_buf;
          p_cmd->state     = CMD_LENGTH_H;
          break;
        case CMD_LENGTH_H:
          p_rx_packet->Length |= (rx_buf<<8);
          p_rx_packet->Crc    += rx_buf;
          if(p_rx_packet->Length>>0)
          {
            p_cmd->state = CMD_DATA;
          }
          else
          {
            p_cmd->state = CMD_CRC;
          }
          break;
        case CMD_DATA:
          p_rx_packet->Data[p_rx_packet ->index] = rx_buf;
          p_rx_packet->Crc += rx_buf;
          (p_rx_packet ->index)++;
          if(p_rx_packet ->index == p_rx_packet->Length)
          {
            p_cmd->state = CMD_CRC;
          }
          break;
        case CMD_CRC:
          p_rx_packet->Crc_prev = rx_buf;
          p_cmd->state = CMD_EOF;
          break;
        case CMD_EOF:
          p_rx_packet->Eof = rx_buf;
          if(p_rx_packet->Eof == DEF_CMD_EOF)
          {
            eof_ret = true;
          }
          if(p_rx_packet->Crc == p_rx_packet->Crc_prev)
          {
            crc_ret=true;
          }
          if(p_rx_packet->Seq == p_rx_packet->SeqExpect)
          {
            seq_ret = true;
          }
          if(eof_ret && crc_ret)
          {
            ret= true;
            p_rx_packet->Valid=true;
            p_rx_packet->SeqExpect = (p_rx_packet->Seq) + 1;
          }
          else
          {
            ret=false;
            p_rx_packet->Valid=false;
          }
          p_cmd->state = CMD_STX;
          break;
      }
    }
  }
  return ret;
}





void cmdRespond(cmd_tbl* p_cmd, uint8_t cmdHeader, uint8_t error, uint8_t* p_data, uint16_t length)
{
  uint16_t cmdseq = p_cmd->rx_packet.Seq;
  cmd_packet_tbl* p_packet = &p_cmd->tx_packet;
  p_packet->Crc = 0;
  p_packet->index = 0;
  p_packet->Buf[p_packet->index++] = DEF_CMD_STX;
  p_packet->Buf[p_packet->index++] = cmdHeader;
  p_packet->Buf[p_packet->index++] = (uint8_t)(cmdseq);
  p_packet->Buf[p_packet->index++] = (uint8_t)((cmdseq>>8)&0xFF);
  p_packet->Buf[p_packet->index++] = CMD_MCU_TO_PC;
  p_packet->Buf[p_packet->index++] = error;
  p_packet->Buf[p_packet->index++] = (uint8_t)(length);
  p_packet->Buf[p_packet->index++] = (uint8_t)((length>>8)&0xFF);
  for(int i=0; i<length; i++)
  {
    p_packet->Buf[p_packet->index++] = p_data[i];
  }
  for(int j=1; j<length+8; j++)
  {
    p_packet->Crc += p_packet->Buf[j];
  }
  p_packet->Buf[p_packet->index++] = p_packet->Crc;
  p_packet->Buf[p_packet->index++] = DEF_CMD_EOF;
  uartWrite(p_cmd->ch, &p_packet->Buf[0], p_packet->index);
}


bool cmdSendResp_Wait(cmd_tbl* p_cmd, uint8_t cmdHeader, uint8_t* p_data, uint16_t length)
{
  uint32_t pre_time = millis();
  bool ret = true;
  cmdSend(p_cmd, cmdHeader, p_data, length);
  while(1)
  {
    if(cmdReceivePacket(p_cmd))
    {
      ret = true;
      break;
    }
    if(millis()-pre_time>=cmdTimeout)
    {
      ret = false;
      break;
    }
  }
  return ret;
}


void cmdSend(cmd_tbl* p_cmd, uint8_t cmdHeader, uint8_t* p_data, uint16_t length)
{
  static uint16_t cmdseq = 0;
  cmdseq++;
  cmd_packet_tbl* p_packet = &p_cmd->tx_packet;
  p_packet->Crc = 0;
  p_packet->index = 0;
  p_packet->Buf[p_packet->index++] = DEF_CMD_STX;
  p_packet->Buf[p_packet->index++] = cmdHeader;
  p_packet->Buf[p_packet->index++] = (uint8_t)(cmdseq);
  p_packet->Buf[p_packet->index++] = (uint8_t)((cmdseq>>8)&0xFF);
  p_packet->Buf[p_packet->index++] = CMD_MCU_TO_PC;
  p_packet->Buf[p_packet->index++] = CMD_OK;
  p_packet->Buf[p_packet->index++] = (uint8_t)(length);
  p_packet->Buf[p_packet->index++] = (uint8_t)((length>>8)&0xFF);
  for(int i=0; i<length; i++)
  {
    p_packet->Buf[p_packet->index++] = p_data[i];
  }
  for(int j=1; j<length+8; j++)
  {
    p_packet->Crc += p_packet->Buf[j];
  }
  p_packet->Buf[p_packet->index++] = p_packet->Crc;
  p_packet->Buf[p_packet->index++] = DEF_CMD_EOF;
  uartWrite(p_cmd->ch, &p_packet->Buf[0], p_packet->index);
}
#endif
