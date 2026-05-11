/*
 * ICM20602.c
 *
 *  Created on: 2026. 2. 17.
 *      Author: yougang
 */


#include "ICM20602/ICM20602.h"
#include "spi.h"


Struct_ICM20602 ICM20602;
int gyro_x_offset, gyro_y_offset, gyro_z_offset;
static uint8_t Axis_Data[15];
static uint8_t Gyro_Data[15];
static uint8_t ACC_Data[7];

static uint8_t tx_buf[30];
static uint8_t rx_buf[30];

ICM_STATE ICM_Flag = AxisGyroRaw;
ICM_STATE SPI_Flag = IDLE;


void ICM20602_GPIO_SPI_Initialization(void)
{
  spiOpen(DEF_SPI_ICM20602);
  CHIP_DESELECT(ICM20602);
}

uint8_t  ICM20602_Readbyte(uint8_t reg_addr)
{
  uint8_t tx_data;
  uint8_t rx_data;
  tx_data=(reg_addr | 0x80);
  CHIP_SELECT(ICM20602);
  SPI_PollByte(DEF_SPI_ICM20602, &tx_data, &rx_data, 1);
  SPI_PollByte(DEF_SPI_ICM20602, &tx_data, &rx_data, 1);
  CHIP_DESELECT(ICM20602);
  return rx_data;
}

void ICM20602_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t* p_data)
{
  if(len>30) return;
  CHIP_SELECT(ICM20602);
  tx_buf[0]=(reg_addr | 0x80);
  for(int i=1; i<len+1; i++)
  {
    tx_buf[i] = 0x00;
  }
  SPI_DMABytes(DEF_SPI_ICM20602, &tx_buf[0], p_data, (len+1));
  CHIP_DESELECT(ICM20602);
}

void ICM20602_Writebyte(uint8_t reg_addr, uint8_t val)
{
  uint8_t tx_data;
  uint8_t rx_data;
  tx_data= (reg_addr & 0x7F);
  CHIP_SELECT(ICM20602);
  SPI_PollByte(DEF_SPI_ICM20602, &tx_data, &rx_data, 1);
  SPI_PollByte(DEF_SPI_ICM20602, &val, &rx_data, 1);
  CHIP_DESELECT(ICM20602);
}

#if 0

void ICM20602_Writebytes(uint8_t reg_addr, uint8_t len, uint8_t* tx_data)
{
  tx_data[0]= (reg&0x7F);
  if(len>30) return;
  CHIP_SELECT(ICM20602);
  SPI_DMABytes(DEF_SPI_ICM20602, &TX_data, RX_data, len+1);
  CHIP_DESELECT(ICM20602);
}
#endif

bool ICM20602_Initialization(void)
{
  bool ret=false;
  uint8_t who_am_i=0;
  int16_t accel_raw_data[3]={0};
  int16_t gyro_raw_data[3]={0};

  ICM20602_GPIO_SPI_Initialization();
  printf("Checking ICM20602...");

  // check WHO_AM_I (0x75)
  who_am_i = ICM20602_Readbyte(WHO_AM_I);

  // who am i = 0x12
  if(who_am_i == 0x12)
  {
    ret=true;
  }
  // recheck
  else if(who_am_i != 0x12)
  {
    who_am_i = ICM20602_Readbyte(WHO_AM_I); // check again WHO_AM_I (0x75)

    if (who_am_i != 0x12){
    ret=false;
   }
  }
    // Reset ICM20602
    // PWR_MGMT_1 0x6B
  ICM20602_Writebyte(PWR_MGMT_1, 0x80); //Reset ICM20602
  HAL_Delay(50);
  // PWR_MGMT_1 0x6B
  ICM20602_Writebyte(PWR_MGMT_1, 0x01); // Enable Temperature sensor(bit4-0), Use PLL(bit2:0-01)
                                         // 온도센서 끄면 자이로 값 이상하게 출력됨
  HAL_Delay(50);
  // PWR_MGMT_2 0x6C
  ICM20602_Writebyte(PWR_MGMT_2, 0x38); // Disable Acc(bit5:3-111), Enable Gyro(bit2:0-000)
    //ICM20602_Writebyte( PWR_MGMT_2, 0x00 ); // Enable Acc(bit5:3-000), Enable Gyro(bit2:0-000)
  HAL_Delay(50);
    // set sample rate to 1000Hz and apply a software filter
  ICM20602_Writebyte(SMPLRT_DIV, 0x00);
  HAL_Delay(50);
    // Gyro DLPF Config
    //ICM20602_Writebyte(CONFIG, 0x00); // Gyro LPF fc 250Hz(bit2:0-000)
  ICM20602_Writebyte(CONFIG, 0x05); // Gyro LPF fc 20Hz(bit2:0-100) at 1kHz sample rate
  HAL_Delay(50);
    // GYRO_CONFIG 0x1B
  ICM20602_Writebyte(GYRO_CONFIG, 0x18); // Gyro sensitivity 2000 dps(bit4:3-11), FCHOICE (bit1:0-00)
  HAL_Delay(50);
    // ACCEL_CONFIG 0x1C
  ICM20602_Writebyte(ACCEL_CONFIG, 0x18); // Acc sensitivity 16g
  HAL_Delay(50);
    // ACCEL_CONFIG2 0x1D
  ICM20602_Writebyte(ACCEL_CONFIG2, 0x03); // Acc FCHOICE 1kHz(bit3-0), DLPF fc 44.8Hz(bit2:0-011)
  HAL_Delay(50);

    // Enable Interrupts when data is ready
  ICM20602_Writebyte(INT_ENABLE, 0x01); // Enable DRDY Interrupt
  HAL_Delay(50);

    //printf("gyro bias: %d %d %d\n", gyro_x_offset, gyro_y_offset, gyro_z_offset);

    // Remove Gyro X offset
  //  ICM20602_Writebyte( XG_OFFS_USRH, offset_x>>8 );  // gyro x offset high byte
  //  ICM20602_Writebyte( XG_OFFS_USRL, offset_x ); // gyro x offset low byte
  //
  //  // Remove Gyro Y offset
  //  ICM20602_Writebyte( YG_OFFS_USRH, offset_y>>8 );  // gyro y offset high byte
  //  ICM20602_Writebyte( YG_OFFS_USRL, offset_y ); // gyro y offset low byte
  //
  //  // Remove Gyro Z offset
  //  ICM20602_Writebyte( ZG_OFFS_USRH, offset_z>>8 );  // gyro z offset high byte
  //  ICM20602_Writebyte( ZG_OFFS_USRL, offset_z ); // gyro z offset low byte

  return ret; //OK
}

bool ICM20602_Read(uint8_t state)
{
  bool ret=false;
  ICM_Flag=AxisGyroRaw;
  if(ICM20602_DataReady()==1 && SPI_Flag == IDLE)
  {
    switch(state)
    {
      case AxisRaw:
        ICM20602_Read6AxisRawData();
        ret=true;
        break;
      case AxisGyroRaw:
        ICM20602.gyro_x = ICM20602.gyro_x_raw*2000.f/32768.f;
        ICM20602.gyro_y = ICM20602.gyro_y_raw*2000.f/32768.f; // 2000: sensitivy, 32768 : adc range
        ICM20602.gyro_z = ICM20602.gyro_z_raw*2000.f/32768.f;
        ICM20602_Read3AxisGyroRawData();
        ret=true;
        break;
      case AxisAccRaw:
        ICM20602_Read3AxisAccRawData();
        ret=true;
        break;
    }
  }
  return ret;
}



bool ICM20602_DataReady(void)
{
  bool ret=true;
  if(HAL_GPIO_ReadPin(ICM20602_INT_PORT, ICM20602_INT_PIN) == GPIO_PIN_SET)
  {
    ret=true;
  }
  else
  {
    ret=false;
  }
  return ret;
}




void ICM20602_Read6AxisRawData(void)
{
  SPI_Flag = Active;
  ICM20602_Readbytes(GYRO_XOUT_H, 6, Gyro_Data);
}
void ICM20602_Read3AxisGyroRawData(void)
{
  SPI_Flag = Active;
  ICM20602_Readbytes(GYRO_XOUT_H, 6, Gyro_Data);
}
void ICM20602_Read3AxisAccRawData(void)
{
  SPI_Flag = Active;
   ICM20602_Readbytes(ACCEL_XOUT_H, 6, ACC_Data);
}


void parsing_6AxisRawData(short* accel, short* gyro)
{

  accel[0] = (Axis_Data[1] << 8) | Axis_Data[2];
  accel[1] = (Axis_Data[3] << 8) | Axis_Data[4];
  accel[2] = (Axis_Data[5] << 8) | Axis_Data[6];

  gyro[0] = (Axis_Data[9] << 8) | Axis_Data[10];
  gyro[1] = (Axis_Data[11] << 8) | Axis_Data[12];
  gyro[2] = (Axis_Data[13] << 8) | Axis_Data[14];
}

void parsing_3AxisGyroRawData(short* gyro)
{
  gyro[0] = (Gyro_Data[1] << 8) | Gyro_Data[2];
  gyro[1] = (Gyro_Data[3] << 8) | Gyro_Data[4];
  gyro[2] = (Gyro_Data[5] << 8) | Gyro_Data[6];
}

void parsing_Get3AxisAccRawyData(short* accel)
{
  accel[0] = (ACC_Data[1] << 8) | ACC_Data[2];
  accel[1] = (ACC_Data[3] << 8) | ACC_Data[4];
  accel[2] = (ACC_Data[5] << 8) | ACC_Data[6];
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  if(hspi->Instance == SPI1)
    {
      CHIP_DESELECT(ICM20602);
      switch(ICM_Flag)
      {
        case AxisRaw:
          parsing_6AxisRawData(&ICM20602.acc_x_raw, &ICM20602.gyro_x_raw);
          SPI_Flag=IDLE;
        break;

        case AxisGyroRaw:
         parsing_3AxisGyroRawData(&ICM20602.gyro_x_raw);
         SPI_Flag=IDLE;
         break;

        case AxisAccRaw:
         parsing_Get3AxisAccRawyData(&ICM20602.acc_x_raw);
         SPI_Flag=IDLE;
         break;

        default:
         break;
      }
    }
}
