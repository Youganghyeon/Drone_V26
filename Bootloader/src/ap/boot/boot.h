/*
 * boot.h
 *
 *  Created on: 2026. 7. 27.
 *      Author: yougang
 */

#ifndef AP_BOOT_BOOT_H_
#define AP_BOOT_BOOT_H_


#include "hw_def.h"
#include "cmd.h"

#ifdef _USE_HW_BOOT

#define BOOT_ERROR_LED         0x00
#define BOOT_ERROR_FLASH_WRITE 0x01
#define BOOT_ERROR_FLASH_ERASE 0x02
#define BOOT_ERROR_JUMP_TO_FW  0x03

#define FLASH_START_ADDR      0x8000000


#define BOOT_START_ADDR       FLASH_START_ADDR



#define FLASH_FW_START_ADDR   0x8010000
#define FLASH_FW_END_ADDR     FLASH_FW_START_ADDR + (512-64) * 1024



typedef enum {
  BOOT_CMD_READ_BOOT_VERSION = 0x00,
  BOOT_CMD_READ_BOOT_NAME    ,
  BOOT_CMD_READ_FIRM_VERSION ,
  BOOT_CMD_READ_FIRM_NAME    ,
  BOOT_CMD_FLASH_ERASE       ,
  BOOT_CMD_FLASH_WRITE       ,
  BOOT_CMD_JUMP_TO_FW        ,
  /*---------- FUNC --------------*/
  BOOT_CMD_LED_CONTROL       ,
  BOOT_CMD_MAX_NUM           ,
  BOOT_CMD_NONE              ,
} BootCmd_Header_tbl;
void bootInit(void);
void bootCmdProcess(cmd_tbl* p_cmd, uint8_t ch, uint32_t baud);
void JumpToFw(void);
void bootJumpToFw(cmd_tbl* p_cmd);
bool bootCheckCrc(void);
bool bootCheckFw(void);
#endif
#endif /* AP_BOOT_BOOT_H_ */
