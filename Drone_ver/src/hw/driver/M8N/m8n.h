/*
 * m8n.h
 *
 *  Created on: 2026. 5. 21.
 *      Author: yougang
 */

#ifndef SRC_HW_TELEMETERY_M8N_M8N_H_
#define SRC_HW_TELEMETERY_M8N_M8N_H_

#include "hw_def.h"

#ifdef _USE_HW_M8N

typedef struct
{
  uint8_t CLASS;
  uint8_t ID;
  uint16_t length;

  uint32_t itow;
  int lon;
  int lat;
  int height;
  int hMSL;
  uint32_t hAcc;
  uint32_t vAcc;

  double lon_f64;
  double lat_f64;

}M8N_UBX_NAV_POSLLH;

typedef struct{
  uint8_t buf[20];
  uint8_t m8n_cplt_flag;
} M8N_UBX_RxBuf;

typedef struct M8N
{
  bool isOpen;
  M8N_UBX_RxBuf RxBuf;
  M8N_UBX_NAV_POSLLH posllh;
}M8N_tbl;

void    M8N_Init(void);
bool    M8N_Open(void);
bool    M8N_Parsing(uint8_t * data, M8N_UBX_NAV_POSLLH* posllh);
bool    M8N_Checksum_Check(uint8_t* data, uint8_t len);
bool    M8N_ReceivePacket(M8N_tbl* m8n);
bool    M8N_Write(uint8_t *buf, uint32_t length);
uint8_t M8N_Read(void);
#endif

#endif /* SRC_HW_TELEMETERY_M8N_M8N_H_ */
