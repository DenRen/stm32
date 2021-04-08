#ifndef STEP_MOTOR_PARAMS_H
#define STEP_MOTOR_PARAMS_H

#define STEP_DRIVER_TIMER   TIM6

// ------------------------------

#define PI_URAD      3141592
#define PI_HALF_URAD 1570796
#define PI_QUAD_URAD  785398

// All library in uradians

#define FULL_STEP_FULL_TURN 200
#define INC_STEP            16      // Decay Modes for {1, 2, 4, 8, 16, ...} - Step Increments
#define FREQ_TIMER          10000   // How many times per second the driver is called

#define INC_FULL_TURN       ( FULL_STEP_FULL_TURN * INC_STEP )
#define STEP_ANGLE          ( 2 * PI_URAD / INC_FULL_TURN )

#endif //STEP_MOTOR_PARAMS_H