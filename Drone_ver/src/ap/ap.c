/*
 * ap.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "ap.h"


static LPS22HH_tbl_t LPS22HH;
static ICM20602_tbl_t ICM20602;
static BNO080_tbl BNO080;
static M8N_tbl    M8N;
static ROHS_tbl   ROHS;

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
  (void)file;
  uartWrite(DEF_UART6, (uint8_t*)ptr, (uint32_t)len);
  return len;
}

void EncodeMsg_AHRS(ROHS_tbl* p_rohs, BNO080_tbl* p_bno, LPS22HH_tbl_t* p_lps);

void apInit(void)
{

  uartOpen(DEF_UART5, 115200);    //FSi6AB uart
  uartOpen(DEF_UART1, 115200);  // ROHS
  uartOpen(DEF_UART4, 9600);    // GPS uart
  uartOpen(DEF_UART6, 9600);    //PC uart
  //  buzSetPitch(2000);
  //  delay(1000);
  //  buzSetPitch(1000);
  //  delay(1000);
  buzDeinit();
  timDeinit();

  M8N_Open();
  ROHS_Open(&ROHS, DEF_UART1);

  LPS22HH_Open(&LPS22HH);
  ICM20602_Open(&ICM20602);
  BNO080_Open(&BNO080);
  BNO080_enableRotationVector(&BNO080, 2500);

  AT24C08_Open();

}


void apMain(void)
{
  uint32_t premillis=0;
  uint32_t premillis_ROHS=millis();
  while(1)
  {
    if(millis()-premillis>=500)
    {
      ledToggle(DEF_LED_2);
      ledToggle(DEF_LED_3);
      premillis=millis();
    }

    LPS22HH_GetInfo(&LPS22HH,LPS22HH_GetPress);
    LPS22HH_GetAlt(&LPS22HH, TEMP_CORRECT);
    LPS22HH_GetAltFilt(&LPS22HH);


    ICM20602_GetInfo(&ICM20602, AxisGyroRaw);
    BNO080_ReadInfo(&BNO080);

    M8N_ReceivePacket(&M8N);
    if(M8N.RxBuf.m8n_cplt_flag== 1)
    {
      M8N.RxBuf.m8n_cplt_flag = 0;

      if(M8N_Checksum_Check(&M8N.RxBuf.buf[0], 36) == 1)
      {
        ledToggle(DEF_LED_1);
        M8N_Parsing(&M8N.RxBuf.buf[0], &M8N.posllh);

        //  printf("LAT: %d\tLON: %d\t Height: %d\n", M8N.posllh.lat, M8N.posllh.lon, M8N.posllh.height);
      }
    }

    if(millis()-premillis_ROHS>=20)
    {
      //ROHS_Read();
      EncodeMsg_AHRS(&ROHS, &BNO080, &LPS22HH);
      ROHS_Write(&ROHS,&ROHS.txBuf[0],20);
      premillis_ROHS=millis();
    }

    /*
     * rohsRead();
     *
     */



  }
}


void EncodeMsg_AHRS(ROHS_tbl* p_rohs, BNO080_tbl* p_bno, LPS22HH_tbl_t* p_lps)
{
  BNO080_Angle_tbl* p_bno_angle = &p_bno->BNO080_Angle;
  p_rohs->txBuf[0] = 0x46;
  p_rohs->txBuf[1] = 0x43;
  p_rohs->txBuf[2] = 0x10;

  p_rohs->txBuf[3] = (short)(p_bno_angle->Roll*100);
  p_rohs->txBuf[4] = ((short)(p_bno_angle->Roll*100))>>8;

  p_rohs->txBuf[5] = (short)(p_bno_angle->Roll*100);
  p_rohs->txBuf[6] = ((short)(p_bno_angle->Roll*100))>>8;

  p_rohs->txBuf[7] = (unsigned short)(p_bno_angle->Yaw*100);
  p_rohs->txBuf[8] = ((unsigned short)(p_bno_angle->Yaw*100))>>8;

  p_rohs->txBuf[9] =  (short)(p_lps->baroAltFilt*10);
  p_rohs->txBuf[10] = ((short)(p_lps->baroAltFilt*10))>>8;

  p_rohs->txBuf[11] = 0;//(short)((iBus.RH-1500)*0.1f*100);
  p_rohs->txBuf[12] = 0;//((short)((iBus.RH-1500)*0.1f*100))>>8;

  p_rohs->txBuf[13] = 0;//(short)((iBus.RV-1500)*0.1f*100);
  p_rohs->txBuf[14] = 0;//((short)((iBus.RV-1500)*0.1f*100))>>8;

  p_rohs->txBuf[15] = 0;//(unsigned short)((iBus.LH-1000)*0.36f*100);
  p_rohs->txBuf[16] = 0;//((unsigned short)((iBus.LH-1000)*0.36f*100))>>8;

  p_rohs->txBuf[17] = 0x00;
  p_rohs->txBuf[18] = 0x00;

  p_rohs->txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    p_rohs->txBuf[19] = p_rohs->txBuf[19] - p_rohs->txBuf[i];
  }
}
