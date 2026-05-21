/*
 * AT24C08.h
 *
 *  Created on: 2026. 5. 20.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_AT24C08_H_
#define SRC_COMMON_HW_INCLUDE_AT24C08_H_

#include "hw_def.h"

bool AT24C08_isInit(void);
bool AT24C08_isOpen(void);
bool AT24C08_Open(void);
void AT24C08_Init(void);
void EP_PIDGain_Write(uint8_t id, float PGain, float IGain, float DGain);
bool EP_PIDGain_Read(uint8_t id, float* PGain, float* IGain, float* DGain);

#endif /* SRC_COMMON_HW_INCLUDE_AT24C08_H_ */
