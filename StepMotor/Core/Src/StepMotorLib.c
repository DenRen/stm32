#include "StepMotorLib.h"
#include "StepMotorParams.h"

#include "tim.h"
#include <stdint.h>

driver_memory_t driver_memory = {0};

void ST_Step_Driver () {
    static uint32_t counter = 0;

    if (counter++ == 0 || counter == 10000) {
        LL_TIM_EnableCounter (TIM2);
        counter = 0;

        LL_TIM_DisableCounter (STEP_DRIVER_TIMER);
    }

    /*
    if (counter == 0) {
        if (driver_memory.number_steps) {
            ++counter;
            --driver_memory.number_steps;

            LL_TIM_EnableCounter (TIM2);
        } else {
            LL_TIM_DisableCounter (STEP_DRIVER_TIMER);
            LL_TIM_DisableCounter (TIM2);
        }
    } else {
        if (++counter == driver_memory.timer_counter)
            counter = 0;
    }
    */
}