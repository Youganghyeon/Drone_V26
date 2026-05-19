/*
 * hw.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#include "hw_def.h"
#include "uart.h"
#include "led.h"
#include "spi.h"
#include "tim.h"
#include "buzzer.h"
#include "ICM20602/ICM20602.h"
#include "LPS22HH/LPS22HH.h"
#include "BNO080/BNO080.h"
void hwInit(void);

#endif /* SRC_HW_HW_H_ */
