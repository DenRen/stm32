#include "axis.h"
#include "StepMotorParams.h"

#include "main.h"

#include <stdlib.h>
#include <stdint.h>

// All usec and urad

__STATIC_INLINE uint32_t DeltaTime2TimerTick (uint32_t delta_time) {
    return delta_time * FREQ_TIMER / 1000000;
}

int AxisRotate (axis_t axis, int urad, int usec) {
    if (axis.motor == NULL) {
        return -1;
    }

    const uint32_t number_steps = urad / STEP_ANGLE;
    const uint32_t delta_time   = usec / number_steps;

    driver_memory.number_steps = number_steps;
    driver_memory.timer_counter = DeltaTime2TimerTick (delta_time);

    ST_SetDir_ClockWise (*axis.motor);
    
    LL_TIM_EnableCounter (STEP_DRIVER_TIMER);

    return 0;
}