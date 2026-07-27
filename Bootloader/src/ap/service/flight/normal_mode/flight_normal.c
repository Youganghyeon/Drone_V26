/*
 * flight_mode_normal.c
 */
#include "flight_if.h"
#include "module.h"

static void normal_reset_pid(FlightPidCtx_t* ctx)
{
    NORMAL_Reset_All_PID_Integrator(ctx->pid_roll, ctx->pid_pitch,
                                     ctx->pid_yawHeading, ctx->pid_yawRate);
}

static uint32_t normal_calc_throttle_base(DroneTm_tbl* tm, FlightPidCtx_t* ctx)
{
    (void)ctx;  // Normal 모드는 PID ctx를 안 씀 → 미사용 경고 방지
    return MOTOR_STOP + 500 + (tm->setthrottle - 1000) * 10;
}

const FlightMode_If_t FlightMode_Normal = {
    .entry               = NULL,
    .reset_pid           = normal_reset_pid,
    .calc_throttle_base  = normal_calc_throttle_base,
    .update              = NULL,
    .update_1ms          = NULL,
    .update_20ms         = NULL,   // Normal 모드는 20ms마다 할 일 없음
};
