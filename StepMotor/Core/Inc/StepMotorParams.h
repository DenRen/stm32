#ifndef STEP_MOTOR_PARAMS_H
#define STEP_MOTOR_PARAMS_H

// All library in uradians and useconds (sometimes nanoseconds)
// u - micro, n - nano

// Timers -----------------------------------------------------------------------------------------

#define SM_DRIVER_TIMER     TIM6
#define SM_PULSE_TIMER_1    TIM2

#define SM_1_TIM_CHANNEL    LL_TIM_CHANNEL_CH1

// Math constants ---------------------------------------------------------------------------------

#define PI_URAD      3141592
#define PI_HALF_URAD 1570796
#define PI_QUAD_URAD  785398

// Step motor params ------------------------------------------------------------------------------

#define NUMBER_STEP_MOTORS 1

#define FULL_STEP_FULL_TURN 200
#define INC_STEP            2       // Decay Modes for {1, 2, 4, 8, 16, ...} - Step Increments
#define FREQ_TIMER          10000   // How many times per second the driver is called

// Pulse parameter --------------------------------------------------------------------------------
#define MIN_PULSE_LEN               10
#define PULSE_TIMER_FREQ            1000000
#define PULSE_TIMER_AUTORELOAD      99
#define PULSE_TIMER_COMPARE_VALUE   20

#define INC_FULL_TURN       ( FULL_STEP_FULL_TURN * INC_STEP )
#define STEP_ANGLE          ( 2 * PI_URAD / INC_FULL_TURN )

// In nanoseconds
#define PULSE_LEN_NSEC      ((1000 * 1000 * 1000 * PULSE_TIMER_COMPARE_VALUE) / PULSE_TIMER_FREQ)
#define MIN_PULSE_LEN_NSEC  (1000 * MIN_PULSE_LEN)

// Check on correct input value -------------------------------------------------------------------
#if PULSE_TIMER_COMPARE_VALUE >= PULSE_TIMER_AUTORELOAD
    #error "PULSE_TIMER_COMPARE >= PULSE_TIMER_AUTORELOAD"
#endif

#if PULSE_LEN_NSEC < MIN_PULSE_LEN_NSEC
    #error "The timer generates too short pulses!"
#endif

int InitializeDriverStepMotors ();

#endif //STEP_MOTOR_PARAMS_H