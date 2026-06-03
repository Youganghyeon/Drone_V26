/*
 * i2c.c
 *
 *  Created on: 2026. 5. 20.
 *      Author: yougang
 */

#include "i2c.h"

#ifdef _USE_HW_I2C
I2C_HandleTypeDef hi2c1;

#define i2cTimeout       100
#define i2cTrial         3
typedef struct{
  I2C_HandleTypeDef* i2cHandle;
  bool isInit;
}I2C_tbl_t;

I2C_tbl_t i2c_tbl[MAX_I2C_CH];

void i2cInit(void)
{
  for(uint8_t i=0; i<MAX_I2C_CH; i++)
  {
    i2c_tbl[i].isInit=false;
  }
}

bool i2cOpen(uint8_t ch)
{
  I2C_HandleTypeDef* p_i2c_handle;
  bool ret= true;
  switch(ch)
  {
    case _DEF_I2C1:
      i2c_tbl[ch].i2cHandle = &hi2c1;
      p_i2c_handle = i2c_tbl[ch].i2cHandle;
      p_i2c_handle->Instance = I2C1;
      p_i2c_handle->Init.ClockSpeed = 400000;
      p_i2c_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
      p_i2c_handle->Init.OwnAddress1 = 0;
      p_i2c_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
      p_i2c_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      p_i2c_handle->Init.OwnAddress2 = 0;
      p_i2c_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      p_i2c_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
      if (HAL_I2C_Init(p_i2c_handle) == HAL_OK)
      {
        ret= true;
        i2c_tbl[ch].isInit=true;
      }
      else
      {
        ret= false;
      }
      break;
    default:
      break;
  }
  return ret;
}


bool Isi2cInit(uint8_t ch)
{
  return i2c_tbl[ch].isInit;
}

bool Isi2cReady(uint8_t ch , uint16_t dev_addr)
{
  bool ret=false;
  I2C_tbl_t* p_i2c = &i2c_tbl[ch];
  if(HAL_I2C_IsDeviceReady(p_i2c->i2cHandle, (uint16_t)(dev_addr<<1), i2cTrial, i2cTimeout) == HAL_OK)
  {
    ret= true;
  }
  return ret;
}

bool i2cWriteData(uint8_t ch, uint16_t dev_addr, uint8_t* pData, uint16_t Size)
{
  bool ret=false;
  I2C_tbl_t* p_i2c = &i2c_tbl[ch];
  HAL_StatusTypeDef res_ret;
  res_ret=HAL_I2C_Master_Transmit(p_i2c->i2cHandle, (uint16_t)(dev_addr<<1), pData, Size, i2cTimeout);
  if(res_ret == HAL_OK)
  {
    ret=true;
  }
  return ret;
}

bool i2cReadData(uint8_t ch, uint16_t dev_addr, uint8_t *pData, uint16_t Size)
{
  bool ret=false;
  I2C_tbl_t* p_i2c = &i2c_tbl[ch];
  HAL_StatusTypeDef res_ret;
  res_ret= HAL_I2C_Master_Receive(p_i2c->i2cHandle,  (uint16_t)(dev_addr<<1), pData, Size,  i2cTimeout);
  if(res_ret == HAL_OK)
  {
    ret=true;
  }
  return ret;
}



bool i2cWriteReg8B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t* pData, uint16_t length)
{
  bool ret=false;
  HAL_StatusTypeDef res_ret;
  I2C_tbl_t* p_i2c = &i2c_tbl[ch];
  res_ret= HAL_I2C_Mem_Write(p_i2c->i2cHandle, (uint16_t)(dev_addr<<1), reg_addr, I2C_MEMADD_SIZE_8BIT, pData, length, i2cTimeout);
  if(res_ret == HAL_OK)
  {
    ret=true;
  }
  return ret;
}

bool i2cReadReg8B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *pData, uint16_t length)
{
  bool ret=false;
  I2C_tbl_t* p_i2c = &i2c_tbl[ch];
  HAL_StatusTypeDef res_ret;
  res_ret= HAL_I2C_Mem_Read(p_i2c->i2cHandle, (uint16_t)(dev_addr<<1), reg_addr, I2C_MEMADD_SIZE_8BIT, pData, length, i2cTimeout);
   if(res_ret == HAL_OK)
   {
     ret=true;
   }
   return ret;
}




bool i2cWrite16B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t* pData, uint16_t length)
{
  bool ret=false;
  HAL_StatusTypeDef res_ret;
  I2C_tbl_t* p_i2c = &i2c_tbl[ch];
  res_ret= HAL_I2C_Mem_Write(p_i2c->i2cHandle, (uint16_t)(dev_addr<<1), reg_addr, I2C_MEMADD_SIZE_16BIT, pData, length, i2cTimeout);
  if(res_ret == HAL_OK)
  {
    ret=true;
  }
  return ret;
}

bool i2cRead16B(uint8_t ch, uint16_t dev_addr, uint16_t reg_addr, uint8_t *pData, uint16_t length)
{
  bool ret=false;
  I2C_tbl_t* p_i2c = &i2c_tbl[ch];
  HAL_StatusTypeDef res_ret;
  res_ret= HAL_I2C_Mem_Read(p_i2c->i2cHandle, (uint16_t)(dev_addr<<1), reg_addr, I2C_MEMADD_SIZE_16BIT, pData, length, i2cTimeout);
   if(res_ret == HAL_OK)
   {
     ret=true;
   }
   return ret;
}


void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    /* USER CODE BEGIN I2C1_MspInit 0 */

    /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
     */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
    /* USER CODE BEGIN I2C1_MspInit 1 */

    /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
    /* USER CODE BEGIN I2C1_MspDeInit 0 */

    /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
     */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

    /* USER CODE BEGIN I2C1_MspDeInit 1 */

    /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#endif
