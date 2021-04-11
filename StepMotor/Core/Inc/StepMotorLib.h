#ifndef STEP_MOTOR_LIB_H
#define STEP_MOTOR_LIB_H

// #include "cmsis_gcc.h"

#include "StepMotorParams.h"
#include "main.h"
#include "stm32f0xx.h"
#include "stm32f0xx_ll_gpio.h"

#include <stdlib.h>

#define SM_DELTA_TIME 1000          // Microseconds

#define SM_DELTA_TIME_ms (SM_DELTA_TIME / 1000)

#define NUM_POS_PER_REV 200         // Number positions shaft per 360 degrees
#define NUM_STEP_PER_mANGLE NUM_POS_PER_REV / (360 * 1000)

// ==========================\\
// Main structures Step Motor ----------------------------------------------------------------
// ==========================//

typedef struct
{
    GPIO_TypeDef* port;
    uint32_t pin;
} wire_t;

typedef struct {
    TIM_TypeDef* TIMx;
    uint32_t CHANNEL_CHx;
} timer_channel_t;

typedef struct
{
    timer_channel_t timer_channel;
    wire_t dir;
    wire_t enable;
} step_motor_t;

// ==================================\\
// Verifiers of the above structures  --------------------------------------------------------
// ==================================//

__STATIC_INLINE int SM_Wire_Verificator (wire_t* wire) {
    return wire       != NULL &&
           wire->port != NULL &&
           wire->pin  <= LL_GPIO_PIN_ALL;
} 
__STATIC_INLINE int SM_Timer_Channel_Verificator (timer_channel_t* timer_channel) {
    return timer_channel              != NULL &&
           timer_channel->TIMx        != NULL &&
           timer_channel->CHANNEL_CHx <= LL_TIM_CHANNEL_CH4;
}
__STATIC_INLINE int SM_Step_Motor_Verificator (step_motor_t* step_motor) {
    return (SM_Timer_Channel_Verificator (&step_motor->timer_channel) == -1) &&
           (SM_Wire_Verificator          (&step_motor->dir)           == -1) &&
           (SM_Wire_Verificator          (&step_motor->enable)        == -1);
}

// ======================================\\
// Enable/Disable and Direction functions ----------------------------------------------------
// ======================================//

// !!! This functions don't check on NULL pointer

__STATIC_INLINE void SM_Enable  (const step_motor_t* stepMotor) {
    LL_GPIO_ResetOutputPin (stepMotor->enable.port, stepMotor->enable.pin);
}
__STATIC_INLINE void SM_Disable (const step_motor_t* stepMotor) {
    LL_GPIO_SetOutputPin (stepMotor->enable.port, stepMotor->enable.pin);
}

__STATIC_INLINE void SM_Set_Dir_ClockWise        (const step_motor_t* stepMotor) {
    LL_GPIO_SetOutputPin (stepMotor->dir.port, stepMotor->dir.pin);
}
__STATIC_INLINE void SM_Set_Dir_CounterClockWise (const step_motor_t* stepMotor) {
    LL_GPIO_ResetOutputPin (stepMotor->dir.port, stepMotor->dir.pin);
}
__STATIC_INLINE void SM_Set_Direction            (const step_motor_t* stepMotor, uint8_t direction) {
    if (direction >= 0)
        SM_Set_Dir_ClockWise (stepMotor);
    else
        SM_Set_Dir_CounterClockWise (stepMotor);
}

// =========================\\
// Main structures Unit Task -----------------------------------------------------------------
// =========================//

// Task for one step motor
typedef struct {
    uint16_t number_steps;
    uint16_t timer_counter;
    uint8_t  direction : 1;
} sm_unit_task_t;

// =====================\\
// Driver user functions ---------------------------------------------------------------------
// =====================//


#endif // STEP_MOTOR_LIB_H