/*
 * rtc.h
 *
 *  Created on: 2026. 6. 2.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_RTC_H_
#define SRC_COMMON_HW_INCLUDE_RTC_H_

#include "hw_def.h"
#ifdef _USE_HW_RTC

void     rtcInit(void);
bool     isRtcInit(void);
bool     rtcBackupRegWrite(uint32_t bkupReg, uint32_t data);
uint32_t rtcBackupRegRead(uint32_t bkupReg);

#endif
#endif /* SRC_COMMON_HW_INCLUDE_RTC_H_ */
