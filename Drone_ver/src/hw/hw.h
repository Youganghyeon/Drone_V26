/*
 * hw.h
 *
 *  Created on: 2026. 2. 8.
 *      Author: yougang
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#include "fs-ia6b.h"
#include "m8n.h"
#include "rohs.h"
#include "hw_def.h"
#include "uart.h"
#include "led.h"
#include "spi.h"
#include "tim.h"
#include "buzzer.h"
#include "i2c.h"
#include "adc.h"
#include "rtc.h"
#include "reset.h"
#include "cli.h"
#include "flash.h"

#include "ICM20602.h"
#include "LPS22HH.h"
#include "BNO080.h"
#include "AT24C08.h"
//#include "service/Servicemsg.h"
void hwInit(void);

#endif /* SRC_HW_HW_H_ */
