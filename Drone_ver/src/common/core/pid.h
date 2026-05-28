/*
 * pid.h
 *
 *  Created on: 2026. 5. 27.
 *      Author: yougang
 */

#ifndef SRC_COMMON_CORE_PID_H_
#define SRC_COMMON_CORE_PID_H_

#include "def.h"

typedef struct{

}Single_PID_tbl;


typedef struct{
Single_PID_tbl* Angle;
Single_PID_tbl* Anglegyro;
}Double_PID_tbl;


#endif /* SRC_COMMON_CORE_PID_H_ */
