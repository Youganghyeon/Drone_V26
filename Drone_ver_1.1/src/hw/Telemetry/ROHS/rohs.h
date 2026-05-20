/*
 * rohs.h
 *
 *  Created on: 2026. 5. 17.
 *      Author: yougang
 */

#ifndef SRC_HW_TELEMETRY_ROHS_ROHS_H_
#define SRC_HW_TELEMETRY_ROHS_ROHS_H_


#include "hw_def.h"

void rohsInit(void);
bool rohsRead(void);
bool rohsWrite(uint8_t* tx_data, uint32_t length);
bool rohsParsing(uint8_t cmd);
#endif /* SRC_HW_TELEMETRY_ROHS_ROHS_H_ */
