/*
 * tim.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "tim.h"

TIM_HandleTypeDef htim3;


bool timInit(void)
{
  bool ret=true;
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
   TIM_MasterConfigTypeDef sMasterConfig = {0};
   TIM_OC_InitTypeDef sConfigOC = {0};

   /* USER CODE BEGIN TIM3_Init 1 */

   /* USER CODE END TIM3_Init 1 */
   htim3.Instance = TIM3;
   htim3.Init.Prescaler = 1000-1;
   htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim3.Init.Period = 21-1;
   htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
   if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
   {
     Error_Handler();
   }
   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
   if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
   {
     Error_Handler();
   }
   if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
   {
     Error_Handler();
   }
   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
   {
     Error_Handler();
   }
   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 10;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN TIM3_Init 2 */

   /* USER CODE END TIM3_Init 2 */
   HAL_TIM_MspPostInit(&htim3);
  return ret;
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PB1     ------> TIM3_CH4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}


bool pwmStart(uint8_t ch)
{
  bool ret=true;
  switch(ch)
  {
    case PWM3_CH4:

      HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
      break;
    default:
      ret=false;
      break;
  }
  return ret;
}

bool pwmPsc(uint8_t ch, uint32_t psc)
{
  bool ret=true;
    switch(ch)
    {
      case PWM3_CH4:
          TIM3->PSC=psc;
        break;
      default:
        ret=false;
        break;
    }
    return ret;
}

bool pwmStop(uint8_t ch)
{
  bool ret=false;
  switch(ch)
  {
    case PWM3_CH4:
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
    ret=true;
    break;
  }
  return ret;
}

bool timDeinit(void)
{
  bool ret=true;
  HAL_TIM_Base_Stop(&htim3);
  return ret;
}
