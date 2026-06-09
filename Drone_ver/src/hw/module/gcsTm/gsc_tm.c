/*
 * GCS_Drone.c
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */


#include "gsc_tm.h"
#include "gsc_tm.h"
#include "rohs.h"
#include "uart.h" // DEF_UART1 사용을 위함

static ROHS_tbl  ROHS;
static gscTm_tbl Gcs_Data;

bool gcsTmInit(void)
{
    bool ret = false;
    if (ROHS_Open(&ROHS, DEF_UART1) == true)
    {
        ret = true;
        Gcs_Data.is_connected = true;
    }
    return ret;
}

void gcsTmUpdate(void)
{
    ROHS_Read(&ROHS);

    if (ROHS.rx_cpltFlag == 1)
    {
        ROHS.rx_cpltFlag = 0;
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
