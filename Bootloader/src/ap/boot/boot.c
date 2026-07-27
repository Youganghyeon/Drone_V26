/*
 * boot.c
 *
 *  Created on: 2026. 6. 27.
 *      Author: yougang
 */


#include "boot.h"
#include "cmd.h"
#include "led.h"
#include "uart.h"
#include "flash.h"

#ifdef _USE_HW_BOOT

typedef struct{
  uint8_t boot_cmd;
  void (*func)(cmd_tbl* p_cmd);
}Boot_tbl;

static void bootFuncNone(cmd_tbl* p_cmd);
static void bootLedControl(cmd_tbl* p_cmd);
static bool bootIsFlashInRange(uint32_t addr, uint32_t length);
static void bootFlashWrite(cmd_tbl* p_cmd);
static void bootFlashErase(cmd_tbl* p_cmd);

Boot_tbl Boot[BOOT_CMD_MAX_NUM] =
    {
        {BOOT_CMD_READ_BOOT_VERSION,  bootFuncNone},
        {BOOT_CMD_READ_BOOT_NAME,     bootFuncNone},
        {BOOT_CMD_READ_FIRM_VERSION,  bootFuncNone},
        {BOOT_CMD_READ_FIRM_NAME,     bootFuncNone},
        {BOOT_CMD_FLASH_ERASE,        bootFlashErase},
        {BOOT_CMD_FLASH_WRITE,        bootFlashWrite},
        {BOOT_CMD_JUMP_TO_FW,         bootJumpToFw},
        {BOOT_CMD_LED_CONTROL,        bootLedControl}
    };


void bootInit(void)
{

}

void bootCmdProcess(cmd_tbl* p_cmd, uint8_t ch, uint32_t baud)
{
  uint8_t boot_num = BOOT_CMD_NONE;
  bool ret= false;
  bool u_ret = false;
  if(IsUartOpen(ch) == false)
  {
    u_ret = uartOpen(ch, baud);
  }
  else
  {
    u_ret = true;
  }
  if(u_ret == true)
  {
    for(int i=0; i<BOOT_CMD_MAX_NUM;i++)
    {
      if(p_cmd->rx_packet.Header == i)
      {
        boot_num = i;
        ret      = true;
        break;
      }
      else
      {
        ret = false;
      }
    }
  }
  if((ret == true) && (Boot[boot_num].func != NULL))
  {
    Boot[boot_num].func(p_cmd);
  }
}
void bootLedControl(cmd_tbl* p_cmd)
{
  uint8_t error_code = CMD_OK;
  cmd_packet_tbl* p_packet = &p_cmd->rx_packet;
  if(p_packet->Data[0]== 1)
  {
    ledOn(DEF_LED_1);
  }
  else if(p_packet->Data[0] == 2)
  {
    ledOff(DEF_LED_1);
  }
  else if(p_packet->Data[0] == 3)
  {
    ledToggle(DEF_LED_1);
  }
  else
  {
    error_code = BOOT_ERROR_LED;
  }
  cmdRespond(p_cmd, BOOT_CMD_LED_CONTROL, error_code, NULL, 0);
}
/*
void BOOT_CMD_READ_BOOT_VERSION(cmd_tbl* p_cmd)
{


}

void BOOT_CMD_READ_BOOT_NAME(cmd_tbl* p_cmd)
{

}*/

void bootFlashWrite(cmd_tbl* p_cmd)
{
  bool ret = false;
  uint8_t error_code = CMD_OK;
  cmd_packet_tbl* p_packet = &p_cmd->rx_packet;
  uint32_t addr= 0;
  uint32_t length= 0;
  addr |= (uint32_t)(p_packet->Data[0]);
  addr |= (uint32_t)((p_packet->Data[1])<<8);
  addr |= (uint32_t)((p_packet->Data[2])<<16);
  addr |= (uint32_t)((p_packet->Data[3])<<24);

  length |= (uint32_t)((p_packet->Data[4])<<0);
  length |= (uint32_t)((p_packet->Data[5])<<8);
  length |= (uint32_t)((p_packet->Data[6])<<16);
  length |= (uint32_t)((p_packet->Data[7])<<24);

  if(bootIsFlashInRange(addr, length))
  {
    if(flashWrite(addr, &p_packet->Data[8], length))
    {
      ret = true;
    }
  }
  if(ret != true)
  {
    error_code = BOOT_ERROR_FLASH_WRITE;
  }

  cmdRespond(p_cmd, BOOT_CMD_FLASH_WRITE, error_code, NULL, 0);
}



void bootFlashErase(cmd_tbl* p_cmd)
{
  bool ret = false;
  uint8_t error_code = CMD_OK;
  cmd_packet_tbl* p_packet = &p_cmd->rx_packet;
  uint32_t addr= 0;
  uint32_t length= 0;
  addr |= (uint32_t)(p_packet->Data[0]);
  addr |= (uint32_t)((p_packet->Data[1])<<8);
  addr |= (uint32_t)((p_packet->Data[2])<<16);
  addr |= (uint32_t)((p_packet->Data[3])<<24);

  length |= (uint32_t)((p_packet->Data[4])<<0);
  length |= (uint32_t)((p_packet->Data[5])<<8);
  length |= (uint32_t)((p_packet->Data[6])<<16);
  length |= (uint32_t)((p_packet->Data[7])<<24);

  if(bootIsFlashInRange(addr, length))
  {
    if(flashErase(addr, length))
    {
      ret = true;
    }
  }
  if(ret != true)
  {
    error_code = BOOT_ERROR_FLASH_ERASE;
  }

  cmdRespond(p_cmd, BOOT_CMD_FLASH_ERASE, error_code, NULL, 0);
}

bool bootIsFlashInRange(uint32_t addr, uint32_t length)
{
  bool ret = false;
  uint32_t flash_start = addr;
  uint32_t flash_end = addr+length-1;
  if((flash_start>=FLASH_FW_START_ADDR) && (flash_start <= FLASH_FW_END_ADDR)
      && (flash_end <=FLASH_FW_END_ADDR) && (flash_end >=FLASH_FW_START_ADDR) )
  {
    ret = true;
  }
  return ret;
}

void bootJumpToFw(cmd_tbl* p_cmd)
{
  bool ret= false;
  uint8_t error_code = BOOT_ERROR_JUMP_TO_FW;
  if(bootCheckFw())
  {
    if(bootCheckCrc())
    {
      ret= true;
      error_code = CMD_OK;
    }
  }
  if(ret == true)
  {
    cmdRespond(p_cmd, BOOT_CMD_JUMP_TO_FW, error_code, NULL, 0);
    delay(500);
    JumpToFw();
  }
}

void JumpToFw(void)
{
  void (**jumpFunc)(void) = (void (**)(void))(FLASH_FW_START_ADDR+4);
  bspDeinit();
  (*jumpFunc)();
}

bool bootCheckFw(void)
{
  bool ret= false;
  uint32_t* checkfw = (uint32_t*)(FLASH_FW_START_ADDR + 4);
  if((*checkfw) >= FLASH_FW_START_ADDR && (*checkfw) <= FLASH_FW_END_ADDR)
  {
    ret = true;
  }
  return ret;
}

bool bootCheckCrc(void)
{
  return true;
}
void bootFuncNone(cmd_tbl* p_cmd)
{
  return;
}

#endif
