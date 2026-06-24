/*
 * flight.h
 *
 *  Created on: 2026. 6. 10.
 *      Author: yougang
 */

#ifndef AP_SERVICE_FLIGHT_FLIGHT_H_
#define AP_SERVICE_FLIGHT_FLIGHT_H_


#include "module.h"
#include "hw_def.h"
#include "pid.h"

typedef enum {
    STATE_DISARMED = 0,
    STATE_ARMING_WARN,
    STATE_ARMED_IDLE,    // 스로틀 최소
    STATE_ARMED_FLIGHT,  // 스로틀 올라옴
    STATE_FAILSAFE
} DroneState;

#define THROTTLE_ARM_MAX    DEF_SET_THROTTLE_MIN + 10
#define THROTTLE_MIN        DEF_SET_THROTTLE_MIN + 30


#define MOTOR_STOP          DEF_ESC_STOP
#define MOTOR_IDLE_MIN      DEF_ESC_STOP+1000
#define MOTOR_MAX           DEF_ESC_MAX

#define YAW_DEADZONE_LOW    SET_YAW_CENTER - 15
#define YAW_DEADZONE_HIGH   SET_YAW_CENTER + 15


bool Drone_Service_Init(void);
void Drone_Arming_Update( Double_PID_tbl* pid_roll,Double_PID_tbl* pid_pitch,
                         Single_PID_tbl* pid_yawHeading, Single_PID_tbl* pid_yawRate);
void Drone_FSM_1ms_Update(Double_PID_tbl* pid_pitch, Double_PID_tbl* pid_roll,
                          Single_PID_tbl* pid_yawHeading, Single_PID_tbl* pid_yawRate);

#endif /* AP_SERVICE_FLIGHT_FLIGHT_H_ */
