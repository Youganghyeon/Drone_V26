/*
 * gcs.h
 *
 *  Created on: 2026. 6. 9.
 *      Author: yougang
 */

#ifndef SRC_HW_GCS_H_
#define SRC_HW_GCS_H_

#include "module_def.h"
#include "hw_def.h"

#define   Max_Rxcplt_num      1
#define   gcsTmRxcplt_Func1   0
#define   None                Max_Rxcplt_num

typedef struct {
    uint8_t  cmd_mode;      // 예: 0=대기, 1=시동, 2=이륙, 3=착륙
    bool     is_connected;  // GCS 연결 상태
    uint8_t* gscTm_rx;
    void     (*gcsTmFunc[Max_Rxcplt_num])(void);
} gscTm_tbl;

// 상위 레이어 인터페이스 함수들
bool gcsTmInit(void);
void gcsTmUpdate(uint8_t ch);
void gcsTmWrite(uint8_t* payload, uint8_t len);
bool IsgscTmInit(void);
void gcsTmRegister_RxFunc(uint8_t ch, void (*func) (void));
gscTm_tbl* gcsTmGetData(void); // 수신된 정제 데이터를 가져가는 Getter

#endif /* SRC_HW_GCS_H_ */
