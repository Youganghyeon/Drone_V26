/*
 * tim.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "tim.h"

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim7;


typedef struct
{
  bool     Flag_1ms;
  bool     Flag_20ms;
  uint32_t counter_20ms;
}TIMER_Flag;

typedef struct{
  uint32_t           channel;
  TIM_HandleTypeDef* htim;
  bool               isInit;
  bool               isOpen;
  TIMER_Flag         timerFlag;
}TIM_tbl_t;

TIM_tbl_t TIM_tbl[TIM_MAX_CH] = {
    {TIM_CHANNEL_4, &htim3, false, false, {false, false, 0}},
    {0,             &htim7, false, false, {false, false, 0}},
};

void timInit(void)
{
  for(int i=0; i<TIM_MAX_CH; i++)
  {
    TIM_tbl[i].isInit=true;
    TIM_tbl[i].isOpen=false;
  }
}

bool timOpen(uint8_t ch)
{
  TIM_tbl_t* p_tim = &TIM_tbl[ch];
  TIM_HandleTypeDef* p_handle = p_tim->htim;
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  switch(ch)
  {
    case DEF_TIM3_CH4:

      /* USER CODE BEGIN TIM3_Init 1 */

      /* USER CODE END TIM3_Init 1 */
      p_handle->Instance = TIM3;
      p_handle->Init.Prescaler = 1000-1;
      p_handle->Init.CounterMode = TIM_COUNTERMODE_UP;
      p_handle->Init.Period = 21-1;
      p_handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
      p_handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
      if (HAL_TIM_Base_Init(p_handle) != HAL_OK)
      {
        Error_Handler();
      }
      sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
      if (HAL_TIM_ConfigClockSource(p_handle, &sClockSourceConfig) != HAL_OK)
      {
        Error_Handler();
      }
      if (HAL_TIM_PWM_Init(p_handle) != HAL_OK)
      {
        Error_Handler();
      }
      sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
      sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
      if (HAL_TIMEx_MasterConfigSynchronization(p_handle, &sMasterConfig) != HAL_OK)
      {
        Error_Handler();
      }
      sConfigOC.OCMode = TIM_OCMODE_PWM1;
      sConfigOC.Pulse = 10;
      sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
      sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
      if (HAL_TIM_PWM_ConfigChannel(p_handle, &sConfigOC, p_tim->channel) != HAL_OK)
      {
        Error_Handler();
      }
      /* USER CODE BEGIN TIM3_Init 2 */

      /* USER CODE END TIM3_Init 2 */
      HAL_TIM_MspPostInit(p_handle);
      p_tim->isOpen= true;
      break;
    case DEF_TIM7_TIMER:
      p_handle->Instance = TIM7;
      p_handle->Init.Prescaler = 41999;
      p_handle->Init.CounterMode = TIM_COUNTERMODE_UP;
      p_handle->Init.Period = 1;
      p_handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
      if (HAL_TIM_Base_Init(p_handle) != HAL_OK)
      {
        Error_Handler();
      }
      sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
      sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
      if (HAL_TIMEx_MasterConfigSynchronization(p_handle, &sMasterConfig) != HAL_OK)
      {
        Error_Handler();
      }
      p_tim->isOpen= true;
      HAL_TIM_Base_Start_IT(p_handle);
      break;
  }

  return true;
}



bool pwmStart(uint8_t ch)
{
  bool ret=false;
  TIM_tbl_t* p_tim = &TIM_tbl[ch];
  TIM_HandleTypeDef* p_handle = p_tim->htim;
  switch(ch)
  {
    case DEF_TIM3_CH4:
      if(HAL_TIM_PWM_Start(p_handle, p_tim->channel)==HAL_OK)
      {
        ret=true;
      }
      break;
    default:
      break;
  }
  return ret;
}

bool pwmPsc(uint8_t ch, uint32_t psc)
{
  bool ret=false;
  switch(ch)
  {
    case DEF_TIM3_CH4:
      TIM3->PSC=psc;
      ret=true;
      break;
    case DEF_TIM7_TIMER:
      TIM7->PSC=psc;
      ret=true;
    default:
      break;
  }
  return ret;
}

bool pwmStop(uint8_t ch)
{
  bool ret=false;
  TIM_tbl_t* p_tim = &TIM_tbl[ch];
  TIM_HandleTypeDef* p_handle = p_tim->htim;
  switch(ch)
  {
    case DEF_TIM3_CH4:
      if(HAL_TIM_PWM_Stop(p_handle, p_tim->channel) == HAL_OK)
      {
        ret=true;
      }
      break;
  }
  return ret;
}

bool timDeinit(uint32_t ch)
{
  bool ret=false;
  TIM_tbl_t* p_tim = &TIM_tbl[ch];
  TIM_HandleTypeDef* p_handle = p_tim->htim;
  if(HAL_TIM_Base_Stop(p_handle) == HAL_OK)
  {
    ret=true;
  }
  return ret;
}

bool Is1msFlag(uint8_t ch)
{
  bool ret = false;
  TIMER_Flag* p_flag = &TIM_tbl[ch].timerFlag;
  if(ch == DEF_TIM7_TIMER)
  {
    if(p_flag->Flag_1ms == true)
    {
      ret= true;
    }
  }
  return ret;
}

bool Is20msFlag(uint8_t ch)
{
  bool ret = false;
  TIMER_Flag* p_flag = &TIM_tbl[ch].timerFlag;
  if(ch == DEF_TIM7_TIMER)
  {
    if(p_flag->Flag_20ms == true)
    {
      ret= true;
    }
  }
  return ret;
}



bool clear1msFlag(uint8_t ch)
{
  TIMER_Flag* p_flag = &TIM_tbl[ch].timerFlag;
   if(ch == DEF_TIM7_TIMER)
   {
     p_flag->Flag_1ms = false;
   }
   return false;
}

bool clear20msFlag(uint8_t ch)
{
  TIMER_Flag* p_flag = &TIM_tbl[ch].timerFlag;
   if(ch == DEF_TIM7_TIMER)
   {
     p_flag->Flag_20ms = false;
   }
   return false;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  if(htim->Instance == TIM_tbl[DEF_TIM7_TIMER].htim->Instance)
  {
    TIM_tbl[DEF_TIM7_TIMER].timerFlag.Flag_1ms = true;
    TIM_tbl[DEF_TIM7_TIMER].timerFlag.counter_20ms = (TIM_tbl[DEF_TIM7_TIMER].timerFlag.counter_20ms+1)%21;
    if(TIM_tbl[DEF_TIM7_TIMER].timerFlag.counter_20ms == 20)
    {
      TIM_tbl[DEF_TIM7_TIMER].timerFlag.Flag_20ms = true;
    }
  }

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
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
  else if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspInit 0 */

  /* USER CODE END TIM7_MspInit 0 */
    /* TIM7 clock enable */
    __HAL_RCC_TIM7_CLK_ENABLE();

    /* TIM7 interrupt Init */
    HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
  /* USER CODE BEGIN TIM7_MspInit 1 */

  /* USER CODE END TIM7_MspInit 1 */
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
  else if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspDeInit 0 */

  /* USER CODE END TIM7_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM7_CLK_DISABLE();

    /* TIM7 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
  /* USER CODE BEGIN TIM7_MspDeInit 1 */

  /* USER CODE END TIM7_MspDeInit 1 */
  }
}
