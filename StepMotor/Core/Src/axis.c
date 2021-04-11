#include "axis.h"

#include "StepMotorParams.h"
#include "StepMotorDriver.h"
#include "StepMotorLib.h"

#include "main.h"
#include <stdlib.h>
#include <stdint.h>

// All usec and urad

__STATIC_INLINE uint32_t DeltaTime2TimerTick (uint32_t delta_time) {
    return (delta_time * FREQ_TIMER) / 1000000;
}

int AxisRotate (uint16_t number_step_motor, int urad, int usec) {
    const uint32_t number_steps = urad / STEP_ANGLE;
    const uint32_t delta_time   = usec / number_steps;

    sm_unit_task_t unit_task = {
        .number_steps  = number_steps,
        .timer_counter = DeltaTime2TimerTick (delta_time),
        .direction     = urad > 0
    };
    
    SM_Driver_Set_Task (number_step_motor, &unit_task);

    LL_TIM_EnableCounter (SM_DRIVER_TIMER);
    
    return 0;
}