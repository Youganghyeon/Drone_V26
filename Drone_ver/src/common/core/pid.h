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

void Double_PID_Calc(Double_PID_tbl* axis, float set_point_angle, float angle/*BNO080 Rotation Angle*/, float rate/*ICM-20602 Angular Rate*/);
void Single_PID_Yaw_Heading_Calc(Single_PID_tbl* axis, float set_point_angle, float angle/*BNO080 Rotation Angle*/, float rate/*ICM-20602 Angular Rate*/);
void Single_PID_Yaw_Rate_Calc(Single_PID_tbl* axis, float set_point_rate, float rate/*ICM-20602 Angular Rate*/);
void Reset_PID_Integrator(Single_PID_tbl* axis);
//void Reset_All_PID_Integrator(Double_PID_tbl* p_roll, Double_PID_tbl* p_pitch,
//                              Single_PID_tbl* p_yaw_heading, Single_PID_tbl* p_yaw_rate
//                              ,Double_PID_tbl* p_alt);
void Double_Altitude_PID_Calculation(Double_PID_tbl* axis,
                                     float set_point_alt,
                                     float alt_meas,
                                     float climb_rate);
void Single_ClimbRate_PID_Calculation(Single_PID_tbl* axis,
                                      float set_point_climb_rate,
                                      float climb_rate);

void NORMAL_Reset_All_PID_Integrator(Double_PID_tbl* p_roll, Double_PID_tbl* p_pitch,
                                     Single_PID_tbl* p_yaw_heading, Single_PID_tbl* p_yaw_rate
                                     );
void ALT_Reset_ALL_PID_Integrator(Double_PID_tbl* p_roll, Double_PID_tbl* p_pitch,
                                  Single_PID_tbl* p_yaw_heading, Single_PID_tbl* p_yaw_rate
                                  ,Double_PID_tbl* p_alt);

#endif
#endif /* SRC_COMMON_CORE_PID_H_ */
