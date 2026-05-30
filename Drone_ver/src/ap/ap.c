/*
 * ap.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "ap.h"


static LPS22HH_tbl_t  LPS22HH;
static ICM20602_tbl_t ICM20602;
static BNO080_tbl     BNO080;
static M8N_tbl        M8N;
static ROHS_tbl       ROHS;
static FSiA6B_tbl     IA6B;

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
  (void)file;
  uartWrite(DEF_UART6, (uint8_t*)ptr, (uint32_t)len);
  return len;
}

static void EncodeMsg_AHRS(ROHS_tbl* p_rohs, BNO080_tbl* p_bno, LPS22HH_tbl_t* p_lps, FSiA6B_tbl* p_iBus);

void apInit(void)
{

  uartOpen(DEF_UART1, 115200);  // ROHS
  uartOpen(DEF_UART5, 115200);    //FSi6AB uart
  uartOpen(DEF_UART6, 9600);    //PC uart
  timOpen(DEF_TIM7_TIMER);
  adcOpen(DEF_ADC1);
  //  buzSetPitch(2000);
  //  delay(1000);
  //  buzSetPitch(1000);
  //  delay(1000);
  buzDeinit();

  M8N_Open();
  ROHS_Open(&ROHS, DEF_UART1);
  FSIA6B_Open(&IA6B);

  LPS22HH_Open(&LPS22HH);
  ICM20602_Open(&ICM20602);
  BNO080_Open(&BNO080);
  BNO080_enableRotationVector(&BNO080, 2500);

  AT24C08_Open();

 // bool EP_PIDGain_Read(PID_All,

}

float    BatVolt=0.0;
uint16_t adcVolt=0;

void apMain(void)
{
  uint32_t premillis=0;
  adcReceive_DMA(DEF_ADC1, (uint32_t*)&adcVolt, 1);
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

    FSIA6B_RecivePacket(&IA6B);
    if(IA6B.ibus_rx_cplt_flag == 1)
    {
      IA6B.ibus_rx_cplt_flag = 0;
      if(FSIA6B_Check_checkSum(&IA6B, 32) == 1)
      {
        FSIA6B_Parsing(&IA6B);
        if(FSIA6B_isFailsafe(&IA6B) == 1)
        {

        }
        else
        {

        }
      }
    }

    if(Is20msFlag(DEF_TIM7_TIMER)==1)
    {

      //ROHS_Read();
      EncodeMsg_AHRS(&ROHS, &BNO080, &LPS22HH, &IA6B);
      ROHS_Write(&ROHS,&ROHS.txBuf[0],20);
      clear20msFlag(DEF_TIM7_TIMER);
    }


    BatVolt = adcVolt * 0.003619f;
  }
}


static void EncodeMsg_AHRS(ROHS_tbl* p_rohs, BNO080_tbl* p_bno, LPS22HH_tbl_t* p_lps, FSiA6B_tbl* p_iBus)
{
  BNO080_Angle_tbl* p_bno_angle = &p_bno->BNO080_Angle;
  p_rohs->txBuf[0] = 0x46;
  p_rohs->txBuf[1] = 0x43;
  p_rohs->txBuf[2] = 0x10;

  p_rohs->txBuf[3] = (int16_t)(p_bno_angle->Roll*100);
  p_rohs->txBuf[4] = ((int16_t)(p_bno_angle->Roll*100))>>8;

  p_rohs->txBuf[5] = (int16_t)(p_bno_angle->Pitch*100);
  p_rohs->txBuf[6] = ((int16_t)(p_bno_angle->Pitch*100))>>8;

  p_rohs->txBuf[7] = (uint16_t)(p_bno_angle->Yaw*100);
  p_rohs->txBuf[8] = ((uint16_t)(p_bno_angle->Yaw*100))>>8;

  p_rohs->txBuf[9] =  (int16_t)(p_lps->baroAltFilt*10);
  p_rohs->txBuf[10] = ((int16_t)(p_lps->baroAltFilt*10))>>8;

  p_rohs->txBuf[11] = (int16_t)((p_iBus->RH-1500)*0.1f*100);
  p_rohs->txBuf[12] = ((int16_t)((p_iBus->RH-1500)*0.1f*100))>>8;

  p_rohs->txBuf[13] = (int16_t)((p_iBus->RV-1500)*0.1f*100);
  p_rohs->txBuf[14] = ((int16_t)((p_iBus->RV-1500)*0.1f*100))>>8;

  p_rohs->txBuf[15] = (uint16_t)((p_iBus->LH-1000)*0.36f*100);
  p_rohs->txBuf[16] = ((uint16_t)((p_iBus->LH-1000)*0.36f*100))>>8;

  p_rohs->txBuf[17] = 0x00;
  p_rohs->txBuf[18] = 0x00;

  p_rohs->txBuf[19] = 0xff;

  for(int i=0;i<19;i++)
  {
    p_rohs->txBuf[19] = p_rohs->txBuf[19] - p_rohs->txBuf[i];
  }
}
