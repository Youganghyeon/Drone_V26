/*
 * flash.h
 *
 *  Created on: 2026. 6. 2.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_FLASH_H_
#define SRC_COMMON_HW_INCLUDE_FLASH_H_

#include "hw_def.h"

#ifdef _USE_HW_FLASH
typedef enum{
  Sector_0=0x00,
  Sector_1,
  Sector_2,
  Sector_3,
  Sector_4,
  Sector_5,
  Sector_6,
  Sector_7,
  Sector_8,
  Sector_9,
  Sector_10,
  Sector_11,
  Sector_Max_num
}Sector_num_tbl;

void flashInit(void);
bool isFlashInit(void);
bool flashErase(uint32_t addr, uint32_t length);
bool flashWrite(uint32_t addr, uint8_t* p_data, uint32_t length);
bool flashRead(uint32_t addr, uint8_t* data, uint32_t length);
#endif
#endif /* SRC_COMMON_HW_INCLUDE_FLASH_H_ */
