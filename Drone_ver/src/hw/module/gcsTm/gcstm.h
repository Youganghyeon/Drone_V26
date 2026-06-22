/*
 * gcs.h
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */

#ifndef SRC_HW_GCS_H_
#define SRC_HW_GCS_H_

#include "hw_def.h"
typedef struct {
    uint8_t  cmd_mode;      // 예: 0=대기, 1=시동, 2=이륙, 3=착륙
    bool     is_connected;  // GCS 연결 상태
} gscTm_tbl;

// 상위 레이어 인터페이스 함수들
bool gcsTmInit(void);
void gcsTmUpdate(void); // 수신 데이터를 파싱해서 정제 데이터로 넘겨주는 함수 필요
void gcsTmWrite(uint8_t* payload, uint8_t len);
gscTm_tbl* gcsTmGetData(void); // 수신된 정제 데이터를 가져가는 Getter

#endif /* SRC_HW_GCS_H_ */
