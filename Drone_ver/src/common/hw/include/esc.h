/*
 * oneshot.h
 *
 *  Created on: 2026. 6. 1.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_ESC_H_
#define SRC_COMMON_HW_INCLUDE_ESC_H_

#include "hw_def.h"

void escInit(void);
void escOpen(uint8_t ch);
void escWrite(uint8_t ch, uint32_t speed);


#endif /* SRC_COMMON_HW_INCLUDE_ESC_H_ */
