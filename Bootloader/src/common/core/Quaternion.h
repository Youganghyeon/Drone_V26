#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "def.h"

#ifdef _USE_QUATERNION
extern float BNO080_Roll;
extern float BNO080_Pitch;
extern float BNO080_Yaw;

void Quaternion_Update(float* q, float* des_data);
float invSqrt(float x);
#endif
#endif
