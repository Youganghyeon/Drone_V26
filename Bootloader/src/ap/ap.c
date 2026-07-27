/*
 * ap.c
 *
 *  Created on: Dec 6, 2020
 *      Author: baram
 */


#include "ap.h"
#include "boot/boot.h"
#include "module.h"
void cliBoot(cli_args_t *args);

cmd_tbl cmd_t;
static DroneTm_tbl* droneTm;
void apInit(void)
{
  //uartOpen(_DEF_UART1, 115200);
  uartOpen(DEF_UART6, 115200);
  uartOpen(DEF_UART5, 115200);  //FSi6AB uart
  //timOpen(DEF_UART7);
  cmdOpen(&cmd_t, DEF_UART6, 115200);


  droneTmInit();
  droneTm = droneLinkData();

}

void apMain(void)
{
  bool boot_flag = false;
  uint32_t pre_time = millis();
  while(1)
  {


    if(cmdReceivePacket(&cmd_t))
    {
      bootCmdProcess(&cmd_t,DEF_UART6,115200);
    }
    if(millis()-pre_time>500)
    {
      ledToggle(DEF_LED_1);
      ledToggle(DEF_LED_2);
      ledToggle(DEF_LED_3);
    }

    droneTmUpdate();

    if(droneTm->is_Received == true)
    {
      if(droneTm->switch_ch[DEF_SwC] == Switch_low)
      {
        if(bootCheckFw() && bootCheckCrc())
        {
          boot_flag= true;
        }
      }
    }

    if(getResetCount() >4)
    {
      if(bootCheckFw() && bootCheckCrc())
      {
        boot_flag= true;
      }
    }
    if(boot_flag == true)
    {
      delay(200);
      JumpToFw();
    }
  }
}
