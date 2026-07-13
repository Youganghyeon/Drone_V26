/*
 * flight_if.h
 *
 *  Created on: 2026. 7. 4.
 *      Author: yougang
 */

#ifndef AP_INTERFACE_FLIGHT_IF_H_
#define AP_INTERFACE_FLIGHT_IF_H_
#include "module.h"
#include "pid.h"
#include "service_def.h"


typedef struct {
    Double_PID_tbl* pid_roll;
    Double_PID_tbl* pid_pitch;
    Single_PID_tbl* pid_yawHeading;
    Single_PID_tbl* pid_yawRate;
    Double_PID_tbl* pid_alt;
} FlightPidCtx_t;

typedef struct {
    void     (*entry)         (FlightPidCtx_t* ctx);
    void     (*reset_pid)     (FlightPidCtx_t* ctx);
    uint32_t (*calc_throttle_base)(DroneTm_tbl* tm, FlightPidCtx_t* ctx);
    void     (*update)        (FlightPidCtx_t* ctx);
    void     (*update_1ms)    (FlightPidCtx_t* ctx);
    void     (*update_20ms)   (FlightPidCtx_t* ctx);
} FlightMode_If_t;

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
#define MOTOR_IDLE_MIN      DEF_ESC_STOP+500
#define MOTOR_MAX           DEF_ESC_MAX

#define YAW_DEADZONE_LOW    SET_YAW_CENTER - 15
#define YAW_DEADZONE_HIGH   SET_YAW_CENTER + 15
bool Drone_Service_Init(Double_PID_tbl* pid_roll, Double_PID_tbl* pid_pitch,
                        Single_PID_tbl* pid_yawHeading, Single_PID_tbl* pid_yawRate,
                        Double_PID_tbl* pid_alt);
void Drone_Arming_Update(void);
void Drone_FSM_1ms_Update(void);
void Drone_Mode_Select(Control_Mode_t mode);
#endif /* AP_INTERFACE_FLIGHT_IF_H_ */
