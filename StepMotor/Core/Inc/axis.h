#ifndef AXIS_H
#define AXIS_H

#include "StepMotorLib.h"

typedef struct {
    StepMotor* motor;
} axis_t;

int AxisRotate (axis_t axis, int urad, int usec);

#endif // AXIS_H