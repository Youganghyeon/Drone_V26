/*
 * pid.c
 *
 *  Created on: 2026. 5. 27.
 *      Author: yougang
 */


//References:
// https://www.servotecnica.com/en/resources/white-papers-en-mobile/dual-loop-advanced-control-techniques-for-real-world-drivetrains/
// https://controlguru.com/the-cascade-control-architecture/

/**
 * PID control.c
 * @author ChrisP @ M-HIVE

 * This library source code is for cascade double loop pid control for STM32 Drone Development online course.
 *
 * Created by ChrisP(Wonyeob Park) @ M-HIVE Embedded Academy, July, 2020
 * Rev. 1.0
 *
 * Where to take the online course.
 * https://www.inflearn.com/course/STM32CubelDE-STM32F4%EB%93%9C%EB%A1%A0-%EA%B0%9C%EB%B0%9C (Korean language supported only)
 *
 * Where to buy MH-FC V2.2 STM32F4 Drone Flight Controller.
 * https://smartstore.naver.com/mhivestore/products/4961922335
 *
 * https://github.com/ChrisWonyeobPark
 * https://blog.naver.com/lbiith
 * https://cafe.naver.com/mhiveacademy
*/

#include "pid.h"

#ifdef _USE_PID
#define DT 0.001f
#define OUTER_DERIV_FILT_ENABLE 1
#define INNER_DERIV_FILT_ENABLE 1
#define OUT_ERR_SUM_MAX 500
#define OUT_I_ERR_MIN -OUT_ERR_SUM_MAX
#define IN_ERR_SUM_MAX 500
#define IN_I_ERR_MIN -IN_ERR_SUM_MAX



void Double_PID_Calc(Double_PID_tbl* axis, float set_point_angle, float angle/*BNO080 Rotation Angle*/, float rate/*ICM-20602 Angular Rate*/)
{
  Single_PID_tbl* p_inner = &axis->inner;
  Single_PID_tbl* p_outer = &axis->outer;
  /*********** Double PID Outer Begin (Roll and Pitch Angular Position Control) *************/
  p_outer->reference = set_point_angle;                           //Set point of outer PID control
  p_outer->meas_value = angle;                                    //BNO080 rotation angle

  p_outer->error = p_outer->reference - p_outer->meas_value;      //Define error of outer loop
  p_outer->p_result = p_outer->error * p_outer->kp;               //Calculate P result of outer loop

  p_outer->error_sum = p_outer->error_sum + p_outer->error * DT; //Define summation of outer loop

  if(p_outer->error_sum > OUT_ERR_SUM_MAX)
  {
    p_outer->error_sum = OUT_ERR_SUM_MAX;
  }
  else if(p_outer->error_sum < OUT_I_ERR_MIN)
  {
    p_outer->error_sum = OUT_I_ERR_MIN;
  }

  p_outer->i_result = p_outer->error_sum * p_outer->ki;      //Calculate I result of outer loop

  p_outer->error_deriv = -rate;                    //Define derivative of outer loop (rate = ICM-20602 Angular Rate)

#if !OUTER_DERIV_FILT_ENABLE
  axis->out.d_result = axis->out.error_deriv * axis->out.kd;      //Calculate D result of outer loop
#else
  p_outer->error_deriv_filt =  p_outer->error_deriv_filt * 0.4f +p_outer->error_deriv * 0.6f;  //filter for derivative
  p_outer->d_result         =  p_outer->error_deriv_filt * p_outer->kd;                 //Calculate D result of inner loop
#endif

  p_outer->pid_result =  p_outer->p_result +  p_outer->i_result +  p_outer->d_result;  //Calculate PID result of outer loop
  /****************************************************************************************/

  /************ Double PID Inner Begin (Roll and Pitch Angular Rate Control) **************/
  p_inner->reference = p_inner->pid_result;  //Set point of inner PID control is the PID result of outer loop (for double PID control)
  p_inner->meas_value = rate;         //ICM-20602 angular rate

  p_inner->error = p_inner->reference - p_inner->meas_value;  //Define error of inner loop
  p_inner->p_result = p_inner->error * p_inner->kp;     //Calculate P result of inner loop

  p_inner->error_sum = p_inner->error_sum + p_inner->error * DT;  //Define summation of inner loop

  if(p_outer->error_sum > IN_ERR_SUM_MAX) p_outer->error_sum = IN_ERR_SUM_MAX;
  else if(p_outer->error_sum < IN_I_ERR_MIN) p_outer->error_sum = IN_I_ERR_MIN;
  p_inner->i_result = p_inner->error_sum * p_inner->ki;             //Calculate I result of inner loop

  p_inner->error_deriv = -(p_inner->meas_value - p_inner->meas_value_prev) / DT;  //Define derivative of inner loop
  p_inner->meas_value_prev = p_inner->meas_value;                 //Refresh value_prev to the latest value

#if !INNER_DERIV_FILT_ENABLE
  axis->in.d_result = axis->in.error_deriv * axis->in.kd;       //Calculate D result of inner loop
#else
  p_inner->error_deriv_filt = p_inner->error_deriv_filt * 0.5f + p_inner->error_deriv * 0.5f; //filter for derivative
  p_inner->d_result = p_inner->error_deriv_filt * p_inner->kd;                //Calculate D result of inner loop
#endif

  p_inner->pid_result = p_inner->p_result + p_inner->i_result + p_inner->d_result; //Calculate PID result of inner loop
  /****************************************************************************************/
}

void Single_PID_Yaw_Heading_Calc(Single_PID_tbl* axis, float set_point_angle, float angle/*BNO080 Rotation Angle*/, float rate/*ICM-20602 Angular Rate*/)
{
  /*********** Single PID Begin (Yaw Angular Position) *************/
  axis->reference = set_point_angle;  //Set point of yaw heading @ yaw stick is center.
  axis->meas_value = angle;     //Current BNO080_Yaw angle @ yaw stick is center.

  axis->error = axis->reference - axis->meas_value; //Define error of yaw angle control

  if(axis->error > 180.f) axis->error -= 360.f;
  else if(axis->error < -180.f) axis->error += 360.f;

  axis->p_result = axis->error * axis->kp;      //Calculate P result of yaw angle control

  axis->error_sum = axis->error_sum + axis->error * DT; //Define summation of yaw angle control
  axis->i_result = axis->error_sum * axis->ki;      //Calculate I result of yaw angle control

  axis->error_deriv = -rate;            //Define differentiation of yaw angle control
  axis->d_result = axis->error_deriv * axis->kd;  //Calculate D result of yaw angle control

  axis->pid_result = axis->p_result + axis->i_result + axis->d_result; //Calculate PID result of yaw angle control
  /***************************************************************/
}

void Single_PID_Yaw_Rate_Calc(Single_PID_tbl* axis, float set_point_rate, float rate/*ICM-20602 Angular Rate*/)
{
  /*********** Single PID Begin (Yaw Angular Rate Control) *************/
  axis->reference = set_point_rate; //Set point of yaw heading @ yaw stick is not center.
  axis->meas_value = rate;      //Current ICM20602.gyro_z @ yaw stick is not center.

  axis->error = axis->reference - axis->meas_value; //Define error of yaw rate control
  axis->p_result = axis->error * axis->kp;      //Calculate P result of yaw rate control

  axis->error_sum = axis->error_sum + axis->error * DT; //Define summation of yaw rate control
  axis->i_result = axis->error_sum * axis->ki;      //Calculate I result of yaw rate control

  axis->error_deriv = -(axis->meas_value - axis->meas_value_prev) / DT; //Define differentiation of yaw rate control
  axis->meas_value_prev = axis->meas_value;               //Refresh value_prev to the latest value
  axis->d_result = axis->error_deriv * axis->kd;              //Calculate D result of yaw rate control

  axis->pid_result = axis->p_result + axis->i_result + axis->d_result; //Calculate PID result of yaw control
  /*******************************************************************/
}

void Reset_PID_Integrator(Single_PID_tbl* axis)
{
  axis->error_sum = 0;
}

void Reset_All_PID_Integrator(Double_PID_tbl* p_roll, Double_PID_tbl* p_pitch,Single_PID_tbl* p_yaw_heading, Single_PID_tbl* p_yaw_rate)
{
  Reset_PID_Integrator(&p_roll->inner);
  Reset_PID_Integrator(&p_pitch->outer);
  Reset_PID_Integrator(&p_pitch->inner);
  Reset_PID_Integrator(&p_pitch->outer);
  Reset_PID_Integrator(p_yaw_heading);
  Reset_PID_Integrator(p_yaw_rate);
}
#endif
