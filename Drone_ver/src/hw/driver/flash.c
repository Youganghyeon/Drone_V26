/*
 * flash.c
 *
 *  Created on: 2026. 6. 2.
 *      Author: yougang
 */
#include "flash.h"
#include "cli.h"

/* stm32f405 reference
 * RM0090
 * Page 74
 *
 */

#ifdef _USE_HW_FLASH
typedef struct{
  uint32_t sector_start;
  uint32_t sector_len;
  uint32_t sector_end;
  Sector_num_tbl  sector_number;
}flash_tbl;

static flash_tbl flash[Sector_Max_num];

static bool IsflashInSector(Sector_num_tbl sector_num, uint32_t addr, uint32_t length);

static const uint32_t flash_start = 0x08000000;
static bool isInit = false;

#ifdef _USE_DEF_CLI
static void cliFlash(cli_args_t* args);
#endif

void flashInit(void)
{
  uint32_t addr = flash_start;
  for(int i=Sector_0; i<Sector_Max_num; i++)
  {
    flash[i].sector_start =  addr;
    flash[i].sector_number = (Sector_num_tbl)i;
    if(i <= Sector_3)
    {
      flash[i].sector_len = 16*1024;
    }
    else if (i==Sector_4)
    {
      flash[i].sector_len = 64*1024;
    }
    else
    {
      flash[i].sector_len = 128*1024;
    }
    flash[i].sector_end = flash[i].sector_start+flash[i].sector_len-1;
    addr = flash[i].sector_end + 1;
  }
  isInit=true;
#ifdef _USE_DEF_CLI
cliAdd("flash", cliFlash);
#endif
}

bool isFlashInit(void)
{
  return isInit;
}

bool flashErase(uint32_t addr, uint32_t length)
{
  if(isInit != true)
  {
    return false;
  }
  if(length == 0)
  {
    return false;
  }
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t SectorError = 0;

  /*
   * firstSector     : first Sector to Erase
   * sectorCnt       : Erase Sector Count
   * flag_foundFirst : flag is set when found firstSector
   */
  bool     ret             = false;
  bool     flag_foundFirst = false;
  uint16_t firstSector     = 0U;
  uint16_t sectorCnt       = 0U;

  for(int i = Sector_0; i < Sector_Max_num; i++)
  {
    if(IsflashInSector((Sector_num_tbl)i, addr, length) == true)
    {
      if(flag_foundFirst == false)
      {
        firstSector     = (uint16_t)i;
        flag_foundFirst = true;
      }

      sectorCnt++;
    }
  }
  if(flag_foundFirst != true || sectorCnt == 0U)
  {
    return false;
  }

  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = firstSector;
  EraseInitStruct.NbSectors = sectorCnt;
  HAL_FLASH_Unlock();
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) == HAL_OK)
  {
    ret=true;
  }

  HAL_FLASH_Lock();
  return ret;
}

bool flashWrite(uint32_t addr, uint8_t* p_data, uint32_t length)
{
  if(isInit != true)
  {
    return false;
  }
  if(length == 0)
  {
    return false;
  }
  if(p_data == NULL)
  {
    return false;
  }
  bool ret=true;
  HAL_FLASH_Unlock();
  for(uint32_t i=0; i<length; i++)
  {
    uint8_t data = p_data[i];
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr+i, (uint64_t)data) != HAL_OK)
    {
      ret=false;
      break;
    }
  }
  HAL_FLASH_Lock();
  return ret;
}

bool flashRead(uint32_t addr, uint8_t* data, uint32_t length) /* you can read sector range in flash */
{
  if(length ==0 || data == NULL)
  {
    return false;
  }
  bool ret= false;
  if(isInit == true)
  {
    if(addr+length-1<=flash[Sector_11].sector_end && addr>=flash[Sector_0].sector_start)
    {
      ret= true;
    }
  }
  uint8_t* p_data = (uint8_t*)addr;
  if(ret == true)
  {
    for(uint32_t i=0; i<length; i++)
    {
      data[i] = p_data[i];
    }

  }
  return ret;
}


static bool IsflashInSector(Sector_num_tbl sector_num, uint32_t addr, uint32_t length)
{
  bool ret= false;
  uint32_t sector_start = flash[sector_num].sector_start;
  uint32_t sector_end   = flash[sector_num].sector_end;
  uint32_t flash_start = addr;
  uint32_t flash_end = addr +length - 1;
  if((sector_start >= flash_start) && (sector_start <= flash_end))
  {
    ret=true;
  }
  if((sector_end>= flash_start) && (sector_end <= flash_end))
  {
    ret=true;
  }
  if((flash_start >= sector_start) && (flash_start<=sector_end))
  {
    ret=true;
  }
  if((flash_end >= sector_start) && (flash_end <= sector_end))
  {
    ret=true;
  }
  return ret;
}
#ifdef _USE_DEF_CLI
void cliFlash(cli_args_t* args)
{
  bool ret=false;
  if((args->argc == 1) && args->isStr(0, "info"))
  {
    for(int i=0; i<Sector_Max_num; i++)
   {
     cliPrintf("%X \n", flash[i].sector_start);
     cliPrintf("%d KB \n", flash[i].sector_len/1024);
   }
    ret=true;
  }
  else if((args->argc == 3) && args->isStr(0, "read"))
  {
    uint32_t addr = (uint32_t)args->getData(1);
    uint32_t length = (uint32_t)args->getData(2);
    uint8_t buf[length];

     flashRead(addr, &buf[0], length);
     for(int i=0; i<length; i++)
      {
        cliPrintf("0x%X \t 0x%X \t 0x%X\n",(addr+i), buf[i], *((uint8_t *)(addr+i)));
      }
     ret=true;
  }
  else if((args->argc == 3) && args->isStr(0, "write"))
  {
    uint32_t addr = (uint32_t)(args->getData(1));
    uint32_t buf = (uint32_t)(args->getData(2));

    if(flashWrite(addr, (uint8_t *)&buf, 4)==true)
    {
     cliPrintf("Write! \n");

    }
    else
    {
      cliPrintf("Write Fail \n");
    }
    ret=true;
  }
  else if((args->argc == 3) && args->isStr(0, "erase"))
  {
    uint32_t addr = (uint32_t)(args->getData(1));
    uint32_t length = (uint32_t)(args->getData(2));

    if(flashErase(addr, length) == true)
    {
      cliPrintf("Erase ! \n");
    }
    else
    {
      cliPrintf("Erase fail \n");
    }
    ret=true;
  }

  if(ret != true)
   {
     cliPrintf("flash info\n");
     cliPrintf("flash read addr len \n");
     cliPrintf("flash write addr data \n");
     cliPrintf("flash erase addr len \n");
   }
}
#endif
#endif
