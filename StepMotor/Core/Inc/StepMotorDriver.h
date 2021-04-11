#ifndef STEP_MOTOR_DRIVER_H
#define STEP_MOTOR_DRIVER_H

#include "StepMotorLib.h"

typedef struct {
    step_motor_t   step_motor[NUMBER_STEP_MOTORS];
    sm_unit_task_t unit_task [NUMBER_STEP_MOTORS];
    uint32_t       counters  [NUMBER_STEP_MOTORS];
} sm_driver_memory_t;

void ST_Step_Driver ();

void SM_Driver_Reset_Step_Motors ();
void SM_Driver_Reset_Unit_Tasks ();
void SM_Driver_Reset_Counters ();
void SM_Driver_Reset_All ();

int SM_Driver_Init_Step_Motor (uint8_t number_step_motor, step_motor_t* step_motor);
int SM_Driver_Verifier ();

// This function is temporary here
void SM_Driver_Set_Direction (uint16_t number_step_motor, uint8_t direction);

void SM_Driver_Enable_Step_Motors ();
void SM_Driver_Disable_Step_Motors ();

#endif // STEP_MOTOR_DRIVER_H