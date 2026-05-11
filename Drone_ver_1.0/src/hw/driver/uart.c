/*
 * uart.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "uart.h"
#include "qbuffer.h"

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_usart6_rx;

static bool is_open[UART_MAX_CH];

static uint8_t uart4_rx_data;
static uint8_t uart6_rx_data;

static uint8_t uart4_rxbuf[256];
static uint8_t uart6_rxbuf[256];


static qbuffer_t qbuffer[UART_MAX_CH];

bool uartInit(void)
{
  bool ret=true;
  for(int i=0;i<UART_MAX_CH; i++)
  {
    is_open[i]=false;
  }
  return ret;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret=true;
  switch(ch)
  {
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

//      HAL_UART_Receive_IT(&huart4,&uart4_rx_data, 1);
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
      HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

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
    case DEF_UART4:
      status=HAL_UART_Transmit(&huart4, buf, length, 100);
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
    case DEF_UART4:
      qbuffer[ch].rx_in=qbuffer[ch].len-hdma_uart4_rx.Instance->NDTR;
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
    case DEF_UART4:
      baud= huart4.Init.BaudRate;
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
  /* Prevent unused argument(s) compilation warning */
  if(huart->Instance==UART4)
  {
      HAL_UART_Transmit(&huart4, &uart4_rx_data, 1, 10);
      HAL_UART_Receive_IT(&huart4,&uart4_rx_data, 1);
  } else if(huart->Instance==USART6)
  {
      HAL_UART_Transmit(&huart6, &uart6_rx_data, 1, 100);
      HAL_UART_Receive_IT(&huart6,&uart6_rx_data, 1);

  }
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
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
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
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
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART6 DMA Init */
    /* USART6_RX Init */
    hdma_usart6_rx.Instance = DMA2_Stream1;
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

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
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
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    /* UART4 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
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
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    /* USART6 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
}
