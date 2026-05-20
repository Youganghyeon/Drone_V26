/*
 * qbuffer.h
 *
 *  Created on: 2026. 2. 15.
 *      Author: yougang
 */

#ifndef SRC_COMMON_HW_QBUFFER_H_
#define SRC_COMMON_HW_QBUFFER_H_

#include "def.h"

typedef struct{

  uint8_t* p_buf;
  uint32_t rx_in;
  uint32_t rx_out;
  uint32_t len;
}qbuffer_t;

bool qbufferInit(void);
bool qbufferCreate(qbuffer_t *p_node, uint8_t* p_buf, uint32_t length);
bool qbufferWrite(qbuffer_t *p_node, uint8_t* p_data, uint32_t length);
bool qbufferRead(qbuffer_t* p_node, uint8_t* p_data, uint32_t length);
uint32_t qbufferAvailable(qbuffer_t *p_node);
bool qbufferflush(qbuffer_t *p_node);

#endif /* SRC_COMMON_HW_QBUFFER_H_ */
