/*
 * reset.h
 *
 *  Created on: 2026. 6. 2.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_INCLUDE_RESET_H_
#define SRC_COMMON_HW_INCLUDE_RESET_H_

#include "hw_def.h"

#ifdef _USE_HW_RESET
void       resetInit(void);
uint32_t   getResetCount(void);
#endif

#endif /* SRC_COMMON_HW_INCLUDE_RESET_H_ */
