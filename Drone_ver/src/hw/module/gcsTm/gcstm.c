/*
 * GCS_Drone.c
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */

#include "gcstm.h"
#include "rohs.h"

static ROHS_tbl  ROHS;
static gscTm_tbl Gcs_Data;

#include "uart.h"

bool gcsTmInit(void)
{
  bool ret = false;
  if (ROHS_Open(&ROHS, DEF_UART1) == true)
  {
    ret = true;
    Gcs_Data.is_connected = true;
    Gcs_Data.gscTm_rx = &ROHS.rxBuf[0];
    for(int i=0; i<Max_Rxcplt_num; i++)
    {
      Gcs_Data.gcsTmFunc[i] = NULL;
    }
  }
  return ret;
}

bool IsgscTmInit(void)
{
  return Gcs_Data.is_connected;
}

void gcsTmUpdate(uint8_t ch)
{
  ROHS_Read(&ROHS);
  if(ROHS.rx_cpltFlag==1 && (Gcs_Data.gcsTmFunc[ch] != NULL))
  {
    if(ch== None)
    {
      return;
    }
    if(ch< Max_Rxcplt_num)
    {
      Gcs_Data.gcsTmFunc[ch]();
    }
  }
}


void gcsTmRegister_RxFunc(uint8_t ch, void (*func) (void))
{
  if(ch== None)
  {
    return;
  }
  if(ch< Max_Rxcplt_num)
  {
    Gcs_Data.gcsTmFunc[ch] = func;
  }
}


void gcsTmWrite(uint8_t* payload, uint8_t len)
{
  ROHS_Write(&ROHS, payload, len);
}

gscTm_tbl* gcsTmGetData(void)
{
  return &Gcs_Data;
}
