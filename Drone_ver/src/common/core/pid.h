/*
 * pid.h
 *
 *  Created on: 2026. 5. 27.
 *      Author: yougang
 */

#ifndef SRC_COMMON_CORE_PID_H_
#define SRC_COMMON_CORE_PID_H_

#include "def.h"

#ifdef _USE_PID
typedef struct{
  float kp;
  float ki;
  float kd;

  float reference;
  float meas_value;
  float meas_value_prev;
  float error;
  float error_sum;
  float error_deriv;
  float error_deriv_filt;

  float p_result;
  float i_result;
  float d_result;

  float pid_result;
}Single_PID_tbl;


typedef struct{
Single_PID_tbl inner;   //angular velceity
Single_PID_tbl outer;   // angle
}Double_PID_tbl;

#endif
#endif /* SRC_COMMON_CORE_PID_H_ */
