#ifndef STEP_MOTOR_LIB_H
#define STEP_MOTOR_LIB_H

#include "cmsis_gcc.h"

#include "stm32f0xx.h"
#include "stm32f0xx_ll_gpio.h"

#define SM_DELTA_TIME 1000          // Microseconds

#define SM_DELTA_TIME_ms (SM_DELTA_TIME / 1000)

#define NUM_POS_PER_REV 200      // Number positions shaft per 360 degrees
#define NUM_STEP_PER_mANGLE NUM_POS_PER_REV / (360 * 1000)

typedef struct
{
    GPIO_TypeDef* port;
    uint32_t pin;
} wire_t;

typedef struct
{
    wire_t step;
    wire_t dir;
    wire_t enable;
} StepMotor;

typedef enum
{
    CLOCKWISE,
    CTR_CLOCKWISE
} DIR_ROTATE;


__STATIC_INLINE void ST_Enable  (const StepMotor stepMotor) {
    LL_GPIO_ResetOutputPin (stepMotor.enable.port, stepMotor.enable.pin);
}
__STATIC_INLINE void ST_Disable (const StepMotor stepMotor) {
    LL_GPIO_SetOutputPin (stepMotor.enable.port, stepMotor.enable.pin);
}

__STATIC_INLINE void ST_SetDir_ClockWise        (const StepMotor stepMotor, DIR_ROTATE dir) {
    LL_GPIO_SetOutputPin (stepMotor.dir.port, stepMotor.dir.pin);
}
__STATIC_INLINE void ST_SetDir_CounterClockWise (const StepMotor stepMotor, DIR_ROTATE dir) {
    LL_GPIO_ResetOutputPin (stepMotor.dir.port, stepMotor.dir.pin);
}

__STATIC_INLINE void ST_Rotate (const StepMotor stepMotor, uint32_t d_mAngle) { // In milliangles
    uint32_t numSteps = d_mAngle * NUM_STEP_PER_mANGLE;

    const wire_t step = stepMotor.step;

    while (numSteps--) {
        LL_GPIO_SetOutputPin (step.port, step.pin);
        LL_mDelay (SM_DELTA_TIME_ms);

        LL_GPIO_ResetOutputPin (step.port, step.pin);
        LL_mDelay (SM_DELTA_TIME_ms);
    }
}

#endif // STEP_MOTOR_LIB_H