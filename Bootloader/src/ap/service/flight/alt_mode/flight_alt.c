/*
 * flight_alt.c
 *
 *  Created on: 2026. 7. 4.
 *      Author: yougang
 */

#if 0
#include "flight_if.h"
#include "module.h"

static void alt_reset_pid(FlightPidCtx_t* ctx);
static uint32_t alt_calc_throttle_base(DroneTm_tbl* tm, FlightPidCtx_t* ctx);
static void alt_update_20ms(FlightPidCtx_t* ctx);

const FlightMode_If_t FlightMode_Alt = {
    .entry               = NULL,
    .reset_pid           = alt_reset_pid,
    .calc_throttle_base  = alt_calc_throttle_base,
    .update              = NULL,
    .update_1ms          = NULL,
    .update_20ms         = alt_update_20ms,
};


static void alt_reset_pid(FlightPidCtx_t* ctx)
{
    ALT_Reset_ALL_PID_Integrator(ctx->pid_roll, ctx->pid_pitch,
                                  ctx->pid_yawHeading, ctx->pid_yawRate,
                                  ctx->pid_alt);
}

static uint32_t alt_calc_throttle_base(DroneTm_tbl* tm, FlightPidCtx_t* ctx)
{
    (void)tm;
    (void)ctx;
    return altControl_Update();
}

static void alt_update_20ms(FlightPidCtx_t* ctx)
{
    (void)ctx;
    Update_Altitude_Hold();
}
#endif
