/*
 * droneTm.c
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */


#include "dronetm.h"
#include "FS-iA6B.h"


#define Switch_Convert(x)   ((x) < 1300 ? Switch_low : ((x) < 1700 ? Switch_IDLE : Switch_high))
static FSiA6B_tbl     IA6B;
static DroneTm_tbl Drone_Tm;
static bool isInit = false;

bool droneTmInit(void)
{
  bool ret= false;
  if(FSIA6B_Open(&IA6B)==true)
  {
    ret= true;
    isInit=true;
  }
  return ret;
}

bool IsdroneTmInit(void)
{
  return isInit;
}

void droneTmUpdate(void)
{
  FSIA6B_RecivePacket(&IA6B);
  if(IA6B.ibus_rx_cplt_flag == 1)
  {
    IA6B.ibus_rx_cplt_flag = 0;
    if(FSIA6B_Check_checkSum(&IA6B, 32) == 1)
    {
      FSIA6B_Parsing(&IA6B);
      Drone_Tm.setRoll     =     IA6B.RH;
      Drone_Tm.setPitch    =     IA6B.RV;
      Drone_Tm.setthrottle =     IA6B.LV;
      Drone_Tm.setyaw      =     IA6B.LH;
      for(int i=0; i<DEF_SW_MAX; i++)
      {
        Drone_Tm.switch_ch[i] = Switch_Convert(IA6B.SW[i]);
      }
      Drone_Tm.knob_A = IA6B.VrA;
      Drone_Tm.knob_B = IA6B.VrB;
      Drone_Tm.failsafe_status = FSIA6B_isFailsafe(&IA6B);
      Drone_Tm.is_connected = IA6B.isOpen;
      Drone_Tm.is_Received =  true;
    }
  }
}


DroneTm_tbl* droneLinkData(void)
{
  return &Drone_Tm;
}
