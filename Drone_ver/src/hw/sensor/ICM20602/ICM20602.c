/*
 * ICM20602.c
 *
 *  Created on: 2026. 2. 17.
 *      Author: yougang
 */

#include "ICM20602/ICM20602.h"
#include "spi.h"

#define DEF_ICM20602   DEF_HW_ICM20602

static bool  ICM20602_Readbyte(uint8_t reg_addr, uint8_t* rx_data);
static void  ICM20602_Readbytes_DMA(ICM20602_Buf_tbl* p_buf, uint8_t reg_addr, uint8_t len, ICM_MODE_STATE type);
static void  ICM20602_Writebyte(uint8_t reg_addr, uint8_t val);
static void  ICM20602_GpioInit(void);
static void  ICM20602_RxFunc(void);
static void  ICM20602_Read6AxisRawData(ICM20602_Buf_tbl* p_buf);
static void  ICM20602_Read3AxisGyroRawData(ICM20602_Buf_tbl* p_buf);
static void  ICM20602_Read3AxisAccRawData(ICM20602_Buf_tbl* p_buf);

#define chipSelect(ICM20602)    HAL_GPIO_WritePin(ICM20602_SPI_CS_PORT, ICM20602_SPI_CS_PIN, RESET)
#define chipDeselect(ICM20602)  HAL_GPIO_WritePin(ICM20602_SPI_CS_PORT, ICM20602_SPI_CS_PIN, SET)

static volatile uint8_t s_done_idx = 0;
static ICM20602_tbl_t*  s_sensor   = NULL;
static volatile uint8_t data_ready_flag = 0;
ICM_MODE_STATE ICM_Mode = AxisGyroRaw;
SPI_DMA_STATE  ICM_Flag = IDLE;


void ICM20602_GpioInit(void)
{
  spiOpen(DEF_ICM20602);
  chipDeselect(ICM20602);
}

bool ICM20602_Readbyte(uint8_t reg_addr, uint8_t* rx_data)
{
  bool    ret = false;
  uint8_t tx_data, dummy, rx_dummy = 0x00;
  tx_data = (reg_addr | 0x80);
  chipSelect(ICM20602);
  ret = SPI_SendReceive(DEF_ICM20602, &tx_data, &rx_dummy, 1);
  if(ret==true)
  {
    ret = SPI_SendReceive(DEF_ICM20602, &dummy, rx_data,  1);
  }
  chipDeselect(ICM20602);
  return ret;
}

void ICM20602_Readbytes_DMA(ICM20602_Buf_tbl* p_buf, uint8_t reg_addr, uint8_t len, ICM_MODE_STATE type)
{
  if(len > 30) return;

  ICM20602_TxPacket_tbl* p_tx = &p_buf->ICM20602_TxPacket;
  ICM20602_RxPacket_tbl* p_rx = &p_buf->ICM20602_RxPacket[0];//p_buf->write_idx];

  uint8_t* p_rxTarget;
  switch(type)
  {
    case AxisRaw:     p_rxTarget = p_rx->Axis_Data; break;
    case AxisGyroRaw: p_rxTarget = p_rx->Gyro_Data; break;
    case AxisAccRaw:  p_rxTarget = p_rx->ACC_Data;  break;
    default: return;
  }

  p_tx->txBuf[0] = (reg_addr | 0x80);
  for(int i = 1; i < len + 1; i++) p_tx->txBuf[i] = 0x00;

  chipSelect(ICM20602);
  SPI_SendReceive_DMA(DEF_ICM20602, p_tx->txBuf, p_rxTarget, len + 1);
}

void ICM20602_Writebyte(uint8_t reg_addr, uint8_t val)
{
  uint8_t tx_data, rx_data;
  tx_data = (reg_addr & 0x7F);
  chipSelect(ICM20602);
  SPI_SendReceive(DEF_ICM20602, &tx_data, &rx_data, 1);
  SPI_SendReceive(DEF_ICM20602, &val,     &rx_data, 1);
  chipDeselect(ICM20602);
}

bool ICM20602_Init(void)
{
  return true;
}

bool ICM20602_Open(ICM20602_tbl_t* p_sensor)
{
  bool ret = false;
  ICM20602_Buf_tbl* p_buf = &p_sensor->ICM20602_Buf;

  chipDeselect(ICM20602);
  if(IsSpiInit(DEF_ICM20602) != true)
  {
    ICM20602_GpioInit();
  }
  spiRxCallbackRegister(DEF_ICM20602, ICM20602_RxFunc);
  s_sensor         = p_sensor;
  p_buf->write_idx = 0;
  p_buf->read_idx  = 1;

  uint8_t who_am_i = 0;
  ICM20602_Readbyte(WHO_AM_I, &who_am_i);
  if(who_am_i != 0x12)
  {
    ICM20602_Readbyte(WHO_AM_I, &who_am_i);  // 재시도
  }
  ret = (who_am_i == 0x12);

  ICM20602_Writebyte(PWR_MGMT_1,    0x80);
  HAL_Delay(50);  // Reset
  ICM20602_Writebyte(PWR_MGMT_1,    0x01);
  HAL_Delay(50);  // PLL, 온도센서 on
  ICM20602_Writebyte(PWR_MGMT_2,    0x38);
  HAL_Delay(50);  // Acc off, Gyro on
  ICM20602_Writebyte(SMPLRT_DIV,    0x00);
  HAL_Delay(50);  // 1kHz
  ICM20602_Writebyte(CONFIG,        0x05);
  HAL_Delay(50);  // Gyro LPF 20Hz
  ICM20602_Writebyte(GYRO_CONFIG,   0x18);
  HAL_Delay(50);  // 2000dps
  ICM20602_Writebyte(ACCEL_CONFIG,  0x18);
  HAL_Delay(50);  // 16g
  ICM20602_Writebyte(ACCEL_CONFIG2, 0x03);
  HAL_Delay(50);  // Acc LPF 44.8Hz
  ICM20602_Writebyte(INT_ENABLE,    0x01);
  HAL_Delay(50);  // DRDY interrupt

  p_sensor->IsOpen = true;
  return ret;
}

/*
bool ICM20602_GetInfo(ICM20602_tbl_t* p_sensor, uint8_t state)
{
  bool ret = false;
  if(p_sensor == NULL) return false;

  ICM_Mode = state;
  ICM20602_Buf_tbl* p_buf = &p_sensor->ICM20602_Buf;
  bool isDataReady = ICM20602_DataReady();

  if(isDataReady== 1 && ICM_Flag == IDLE)
  {
    switch(state)
    {
      case AxisRaw:     ICM20602_Read6AxisRawData(p_buf);     ret = true; break;
      case AxisGyroRaw: ICM20602_Read3AxisGyroRawData(p_buf); ret = true; break;
      case AxisAccRaw:  ICM20602_Read3AxisAccRawData(p_buf);  ret = true; break;
      default: break;
    }
  }
  else if(ICM_Flag == DONE)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    uint8_t parse_idx    = s_done_idx;
    p_buf->write_idx     = parse_idx ^ 1;
    p_buf->read_idx      = parse_idx;

    // 다음 DMA 먼저 시작 (반대쪽 버퍼에)
    if(isDataReady == 1)
    {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
      switch(state)
      {
        case AxisRaw:     ICM20602_Read6AxisRawData(p_buf);     ret = true; break;
        case AxisGyroRaw: ICM20602_Read3AxisGyroRawData(p_buf); ret = true; break;
        case AxisAccRaw:  ICM20602_Read3AxisAccRawData(p_buf);  ret = true; break;
        default: break;
      }
    }
    if(ret != true) ICM_Flag = IDLE;

    // read_idx 버퍼 파싱 (DMA는 write_idx로 받는 중)
    ICM20602_RxPacket_tbl* p_rx = &p_buf->ICM20602_RxPacket[p_buf->read_idx];
    switch(state)
    {
      case AxisRaw:
        parsing_6AxisRawData(&p_sensor->acc_x_raw, &p_sensor->gyro_x_raw, p_rx);
        break;
      case AxisGyroRaw:
        parsing_3AxisGyroRawData(&p_sensor->gyro_x_raw, p_rx);
        p_sensor->gyro_x = p_sensor->gyro_x_raw * 2000.f / 32768.f;
        p_sensor->gyro_y = p_sensor->gyro_y_raw * 2000.f / 32768.f;
        p_sensor->gyro_z = p_sensor->gyro_z_raw * 2000.f / 32768.f;
        break;
      case AxisAccRaw:
        parsing_Get3AxisAccRawData(&p_sensor->acc_x_raw, p_rx);
        break;
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
  }
  return ret;
}
*/
bool ICM20602_GetInfo(ICM20602_tbl_t* p_sensor, uint8_t state)
{
  bool ret = false;
  if(p_sensor == NULL) return false;

  ICM_Mode = state;
  bool isDataReady = ICM20602_DataReady();
  ICM20602_Buf_tbl* p_buf = &p_sensor->ICM20602_Buf;
  ICM20602_RxPacket_tbl* p_rx = &p_buf->ICM20602_RxPacket[0];
  if(ICM_Flag == IDLE && isDataReady)
  {
    switch(state)
    {
      case AxisRaw:     ICM20602_Read6AxisRawData(p_buf);     break;
      case AxisGyroRaw: ICM20602_Read3AxisGyroRawData(p_buf); break;
      case AxisAccRaw:  ICM20602_Read3AxisAccRawData(p_buf);  break;
      default: break;
    }
  }

  if(ICM_Flag == DONE)
  {
    switch(state)
    {
      case AxisRaw:
        parsing_6AxisRawData(&p_sensor->acc_x_raw, &p_sensor->gyro_x_raw,p_rx);
        break;
      case AxisGyroRaw:
        parsing_3AxisGyroRawData(&p_sensor->gyro_x_raw, p_rx);
        p_sensor->gyro_x = p_sensor->gyro_x_raw * 2000.f / 32768.f;
        p_sensor->gyro_y = p_sensor->gyro_y_raw * 2000.f / 32768.f;
        p_sensor->gyro_z = p_sensor->gyro_z_raw * 2000.f / 32768.f;
        break;
      case AxisAccRaw:
        parsing_Get3AxisAccRawData(&p_sensor->acc_x_raw,p_rx);
        break;
    }
    ICM_Flag = IDLE;
    ret = true;
  }

  return ret;
}

bool ICM20602_DataReady(void)
{
  if(data_ready_flag)
  {
    data_ready_flag = 0;
    return true;
  }
  return false;
}

void parsing_6AxisRawData(short* accel, short* gyro, ICM20602_RxPacket_tbl* p_rx)
{
  accel[0] = (p_rx->Axis_Data[1] << 8) | p_rx->Axis_Data[2];
  accel[1] = (p_rx->Axis_Data[3] << 8) | p_rx->Axis_Data[4];
  accel[2] = (p_rx->Axis_Data[5] << 8) | p_rx->Axis_Data[6];
  gyro[0]  = (p_rx->Axis_Data[9]  << 8) | p_rx->Axis_Data[10];
  gyro[1]  = (p_rx->Axis_Data[11] << 8) | p_rx->Axis_Data[12];
  gyro[2]  = (p_rx->Axis_Data[13] << 8) | p_rx->Axis_Data[14];
}

void parsing_3AxisGyroRawData(short* gyro, ICM20602_RxPacket_tbl* p_rx)
{
  gyro[0] = (p_rx->Gyro_Data[1] << 8) | p_rx->Gyro_Data[2];
  gyro[1] = (p_rx->Gyro_Data[3] << 8) | p_rx->Gyro_Data[4];
  gyro[2] = (p_rx->Gyro_Data[5] << 8) | p_rx->Gyro_Data[6];
}

void parsing_Get3AxisAccRawData(short* accel, ICM20602_RxPacket_tbl* p_rx)
{
  accel[0] = (p_rx->ACC_Data[1] << 8) | p_rx->ACC_Data[2];
  accel[1] = (p_rx->ACC_Data[3] << 8) | p_rx->ACC_Data[4];
  accel[2] = (p_rx->ACC_Data[5] << 8) | p_rx->ACC_Data[6];
}

extern SPI_HandleTypeDef hspi1;

void ICM20602_RxFunc(void)
{
  uint32_t timeout = 100000;  // ← BNO080처럼 timeout 추가
    while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_BSY))
    {
      if(timeout-- == 0) break;
    }
    __HAL_SPI_CLEAR_OVRFLAG(&hspi1);
    s_done_idx = s_sensor->ICM20602_Buf.write_idx;
    chipDeselect(ICM20602);
    ICM_Flag = DONE;
}

void ICM20602_Read6AxisRawData(ICM20602_Buf_tbl* p_buf)
{
  ICM_Flag = Active;
  ICM20602_Readbytes_DMA(p_buf, ACCEL_XOUT_H, 14, AxisRaw);
}

void ICM20602_Read3AxisGyroRawData(ICM20602_Buf_tbl* p_buf)
{
  ICM_Flag = Active;
  ICM20602_Readbytes_DMA(p_buf, GYRO_XOUT_H, 6, AxisGyroRaw);
}

void ICM20602_Read3AxisAccRawData(ICM20602_Buf_tbl* p_buf)
{
  ICM_Flag = Active;
  ICM20602_Readbytes_DMA(p_buf, ACCEL_XOUT_H, 6, AxisAccRaw);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  if(GPIO_Pin == ICM_INT_Pin)
  {
    data_ready_flag = 1;
  }
}
