/*
 * led.c
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */


#include "led.h"

#ifdef _USE_HW_LED

typedef struct {
  GPIO_TypeDef* GPIO_Port;
  uint16_t      GPIO_Pin;
  GPIO_PinState STATE_On;
  GPIO_PinState STATE_Off;
}led_tbl_t;


led_tbl_t led_tbl[LED_MAX] = {
    {GPIOC,LED_1_Pin,GPIO_PIN_SET,GPIO_PIN_RESET},
    {GPIOC,LED_2_Pin,GPIO_PIN_SET,GPIO_PIN_RESET},
    {GPIOC,LED_3_Pin,GPIO_PIN_SET,GPIO_PIN_RESET}
};

bool ledInit(void)
{
  bool ret=true;
   return ret;
}

bool ledOn(uint8_t ch)
{
  bool ret=true;
  HAL_GPIO_WritePin(led_tbl[ch].GPIO_Port,led_tbl[ch].GPIO_Pin,led_tbl[ch].STATE_On);
  if(ch>=LED_MAX) ret=false;
  return ret;
}


bool ledOff(uint8_t ch)
{
  bool ret=true;
  HAL_GPIO_WritePin(led_tbl[ch].GPIO_Port,led_tbl[ch].GPIO_Pin,led_tbl[ch].STATE_Off);
  if(ch>=LED_MAX) ret=false;
  return ret;
}

bool ledToggle(uint8_t ch)
{
  bool ret=true;
  HAL_GPIO_TogglePin(led_tbl[ch].GPIO_Port,led_tbl[ch].GPIO_Pin);
  if(ch>=LED_MAX) ret=false;
  return ret;
}

#endif
