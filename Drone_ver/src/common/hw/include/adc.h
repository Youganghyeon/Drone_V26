/*
 * adc.h
 *
 *  Created on: 2026. 5. 29.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_ADC_H_
#define SRC_COMMON_HW_INCLUDE_ADC_H_


#include "hw_def.h"

#define   ADC_MAX_CH          HW_ADC_MAX_CH
#define   DEF_ADC1            HW_DEF_ADC1

void     adcInit(void);
bool     adcOpen(uint8_t ch);
uint32_t adcReceive(uint8_t ch, uint32_t* des_buf, uint32_t length);
bool     adcReceive_DMA(uint8_t ch, uint32_t* des_buf, uint32_t length);

#endif /* SRC_COMMON_HW_INCLUDE_ADC_H_ */
