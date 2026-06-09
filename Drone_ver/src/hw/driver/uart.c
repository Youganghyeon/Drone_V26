/*
 * uart.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "uart.h"
#include "qbuffer.h"

#ifdef _USE_HW_UART

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart6_rx;

static bool is_open[UART_MAX_CH];

static uint8_t uart1_rxbuf[256];
static uint8_t uart4_rxbuf[256];
static uint8_t uart5_rxbuf[256];
static uint8_t uart6_rxbuf[256];


static qbuffer_t qbuffer[UART_MAX_CH];

bool IsUartOpen(uint8_t ch)
{
  return is_open[ch];
}

bool uartInit(void)
{
  bool ret=true;
  for(int i=0;i<UART_MAX_CH; i++)
  {
    is_open[i]=false;
  }
  return ret;
}

/* UART1:Telemetry
 * UART5:iBus
 * UART4:M8N
   UART6:PC
 */

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret=true;
  switch(ch)
  {
    case DEF_UART1:
      huart1.Instance = USART1;
       huart1.Init.BaudRate = 115200;
       huart1.Init.WordLength = UART_WORDLENGTH_8B;
       huart1.Init.StopBits = UART_STOPBITS_1;
       huart1.Init.Parity = UART_PARITY_NONE;
       huart1.Init.Mode = UART_MODE_TX_RX;
       huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
       huart1.Init.OverSampling = UART_OVERSAMPLING_16;
      HAL_UART_DeInit(&huart1);

      qbufferCreate(&qbuffer[ch], &uart1_rxbuf[0], 256);
      __HAL_RCC_DMA2_CLK_ENABLE();
      HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
      if (HAL_UART_Init(&huart1) != HAL_OK)
      {
        Error_Handler();
        ret=false;
      }
      else
      {
        ret=true;
        is_open[ch]=true;
        if(HAL_UART_Receive_DMA(&huart1,&uart1_rxbuf[0],256) == HAL_OK)
        {
          qbuffer[ch].rx_in=qbuffer[ch].len-hdma_usart1_rx.Instance->NDTR;  //NDTR=CNDTR;
          qbuffer[ch].rx_out=qbuffer[ch].rx_in;
        }
      }
      break;
    case DEF_UART4:
      huart4.Instance = UART4;
      huart4.Init.BaudRate = baud;
      huart4.Init.WordLength = UART_WORDLENGTH_8B;
      huart4.Init.StopBits = UART_STOPBITS_1;
      huart4.Init.Parity = UART_PARITY_NONE;
      huart4.Init.Mode = UART_MODE_TX_RX;
      huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart4.Init.OverSampling = UART_OVERSAMPLING_16;

      HAL_UART_DeInit(&huart4);

      qbufferCreate(&qbuffer[ch], &uart4_rxbuf[0], 256);

      __HAL_RCC_DMA1_CLK_ENABLE();
       HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
       HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

      if (HAL_UART_Init(&huart4) != HAL_OK)
      {
        Error_Handler();
        ret=false;
      }
      else
      {
        ret=true;
        is_open[ch]=true;
        if(HAL_UART_Receive_DMA(&huart4,&uart4_rxbuf[0],256) == HAL_OK)
        {
          qbuffer[ch].rx_in=qbuffer[ch].len-hdma_uart4_rx.Instance->NDTR;  //NDTR=CNDTR;
          qbuffer[ch].rx_out=qbuffer[ch].rx_in;
        }
      }
       //HAL_UART_Receive_IT(&huart4,&uart4_rx_data, 1);
      break;
    case DEF_UART5:
      huart5.Instance = UART5;
      huart5.Init.BaudRate = baud;
      huart5.Init.WordLength = UART_WORDLENGTH_8B;
      huart5.Init.StopBits = UART_STOPBITS_1;
      huart5.Init.Parity = UART_PARITY_NONE;
      huart5.Init.Mode = UART_MODE_TX_RX;
      huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart5.Init.OverSampling = UART_OVERSAMPLING_16;
      HAL_UART_DeInit(&huart5);

      qbufferCreate(&qbuffer[ch], &uart5_rxbuf[0], 256);
      __HAL_RCC_DMA1_CLK_ENABLE();
      HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
      if (HAL_UART_Init(&huart5) != HAL_OK)
      {
        Error_Handler();
        ret=false;
      }
      else
      {
        ret=true;
        is_open[ch]=true;
        if(HAL_UART_Receive_DMA(&huart5,&uart5_rxbuf[0],256) == HAL_OK)
        {
          qbuffer[ch].rx_in=qbuffer[ch].len-hdma_uart5_rx.Instance->NDTR;  //NDTR=CNDTR;
          qbuffer[ch].rx_out=qbuffer[ch].rx_in;
        }
      }
      break;

    case DEF_UART6:
      huart6.Instance = USART6;
      huart6.Init.BaudRate = baud;
      huart6.Init.WordLength = UART_WORDLENGTH_8B;
      huart6.Init.StopBits = UART_STOPBITS_1;
      huart6.Init.Parity = UART_PARITY_NONE;
      huart6.Init.Mode = UART_MODE_TX_RX;
      huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart6.Init.OverSampling = UART_OVERSAMPLING_16;

      HAL_UART_DeInit(&huart6);

      qbufferCreate(&qbuffer[ch], &uart6_rxbuf[0], 256);

      __HAL_RCC_DMA2_CLK_ENABLE();
      HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

      if (HAL_UART_Init(&huart6) != HAL_OK)
      {
        Error_Handler();
        ret=false;
      }
      else
      {
        ret=true;
        is_open[ch]=true;
        if(HAL_UART_Receive_DMA(&huart6,&uart6_rxbuf[0],256) == HAL_OK)
        {
          qbuffer[ch].rx_in=qbuffer[ch].len-hdma_usart6_rx.Instance->NDTR;  //NDTR=CNDTR;
          qbuffer[ch].rx_out=qbuffer[ch].rx_in;
        }
      }

      //      HAL_UART_Receive_IT(&huart6,&uart6_rx_data,1);
      break;
  }
  return ret;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret;
  qbufferRead(&qbuffer[ch],&ret,1);
  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *buf, uint32_t length)
{
  uint32_t ret=0;
  HAL_StatusTypeDef status;
  switch(ch)
  {
    case DEF_UART1:
      status=HAL_UART_Transmit(&huart1, buf, length, 100);
      if(status == HAL_OK)
      {
        ret=length;
      }
      break;
    case DEF_UART4:
      status=HAL_UART_Transmit(&huart4, buf, length, 100);
      if(status == HAL_OK)
      {
        ret=length;
      }
      break;
    case DEF_UART5:
      status=HAL_UART_Transmit(&huart5, buf, length, 100);
      if(status == HAL_OK)
      {
        ret=length;
      }
      break;
    case DEF_UART6:
      status=HAL_UART_Transmit(&huart6, buf, length, 100);
      if(status==HAL_OK)
      {
        ret=length;
      }
      break;
  }
  return ret;
}

uint32_t uartWrite_IT(uint8_t ch, uint8_t *buf, uint32_t length)
{
  uint32_t ret=0;
  HAL_StatusTypeDef status;
  switch(ch)
  {
    case DEF_UART1:
      status=HAL_UART_Transmit_IT(&huart1, buf, length);
      if(status == HAL_OK)
      {
        ret=length;
      }
      break;
    case DEF_UART4:
      status=HAL_UART_Transmit_IT(&huart4, buf, length);
      if(status == HAL_OK)
      {
        ret=length;
      }
      break;
    case DEF_UART5:
      status=HAL_UART_Transmit_IT(&huart5, buf, length);
      if(status == HAL_OK)
      {
        ret=length;
      }
      break;
    case DEF_UART6:
      status=HAL_UART_Transmit_IT(&huart6, buf, length);
      if(status==HAL_OK)
      {
        ret=length;
      }
      break;
  }
  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char* fmt,...)
{
  uint32_t ret;
  char buf[256];

  va_list args;
  va_start(args, fmt);

  int len = vsnprintf(buf, sizeof(buf), fmt, args);
  ret= uartWrite(ch, (uint8_t *)buf, len);
  va_end(args);

  return ret;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret;
  switch(ch)
  {
    case DEF_UART1:
      qbuffer[ch].rx_in=qbuffer[ch].len-hdma_usart1_rx.Instance->NDTR;
      ret=qbufferAvailable(&qbuffer[ch]);
      break;
    case DEF_UART4:
      qbuffer[ch].rx_in=qbuffer[ch].len-hdma_uart4_rx.Instance->NDTR;
      ret=qbufferAvailable(&qbuffer[ch]);
      break;
    case DEF_UART5:
      qbuffer[ch].rx_in=qbuffer[ch].len-hdma_uart5_rx.Instance->NDTR;
      ret=qbufferAvailable(&qbuffer[ch]);
      break;
    case DEF_UART6:
      qbuffer[ch].rx_in=qbuffer[ch].len-hdma_usart6_rx.Instance->NDTR;
      ret=qbufferAvailable(&qbuffer[ch]);
      break;
  }
  return ret;
}

uint32_t GetBaud(uint8_t ch)
{
  uint32_t baud;
  switch(ch)
  {
    case DEF_UART1:
      baud= huart1.Init.BaudRate;
      break;
    case DEF_UART4:
      baud= huart4.Init.BaudRate;
      break;
    case DEF_UART5:
      baud= huart5.Init.BaudRate;
      break;
    case DEF_UART6:
      baud= huart6.Init.BaudRate;
      break;
  }
  return baud;
}


//int _write(int file, char *ptr, int len)
//{
//  HAL_UART_Transmit(&huart6, (uint8_t *)ptr, len, 10);
//  return len;
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning *
   *
   */
  if(huart->Instance==USART1)
  {

  }
  else if(huart->Instance==UART4)
  {

  } else if(huart->Instance==USART6)
  {

  }
}


void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = M8N_TX_Pin|M8N_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* UART4 DMA Init */
    /* UART4_RX Init */
    hdma_uart4_rx.Instance = DMA1_Stream2;
    hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_rx.Init.Mode = DMA_CIRCULAR;
    hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart4_rx);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    GPIO_InitStruct.Pin = iBus_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(iBus_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = iBus_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(iBus_RX_GPIO_Port, &GPIO_InitStruct);

    /* UART5 DMA Init */
    /* UART5_RX Init */
    hdma_uart5_rx.Instance = DMA1_Stream0;
    hdma_uart5_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_uart5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart5_rx.Init.Mode = DMA_CIRCULAR;
    hdma_uart5_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart5_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart5_rx);

  /* USER CODE BEGIN UART5_MspInit 1 */

  /* USER CODE END UART5_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream5;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = DEBUG_TX_Pin|DEBUG_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART6 DMA Init */
    /* USART6_RX Init */
    hdma_usart6_rx.Instance = DMA2_Stream2;
    hdma_usart6_rx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart6_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart6_rx);

  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOC, M8N_TX_Pin|M8N_RX_Pin);

    /* UART4 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();

    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    HAL_GPIO_DeInit(iBus_TX_GPIO_Port, iBus_TX_Pin);

    HAL_GPIO_DeInit(iBus_RX_GPIO_Port, iBus_RX_Pin);

    /* UART5 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    HAL_GPIO_DeInit(GPIOC, DEBUG_TX_Pin|DEBUG_RX_Pin);

    /* USART6 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
}

#endif
