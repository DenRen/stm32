#include "StepMotorLib.h"
#include "StepMotorParams.h"

#include "tim.h"
#include <stdint.h>

driver_memory_t driver_memory = {0};

__STATIC_INLINE void SendPulse (TIM_TypeDef* TIMx, uint32_t TIM_CHANNEL_CHx, uint32_t len_pulse) {
    LL_TIM_OC_DisablePreload (TIMx, TIM_CHANNEL_CHx);   // To instantly change timer settings
    LL_TIM_OC_SetCompareCH1 (TIMx, len_pulse);          // Set length pulse
    
    LL_TIM_OC_EnablePreload (TIMx, TIM_CHANNEL_CHx);    // So that after the pulse, the output is 0
    LL_TIM_OC_SetCompareCH1 (TIMx, 0);                  // Set 0 to CCRx

    LL_TIM_EnableCounter (TIMx);                        // Start generate pulse
}

void ST_Step_Driver () {
    static uint32_t counter = -1;   // Сделать через TIM6->CNT
    
    if (++counter == driver_memory.timer_counter || counter == 0) {
        counter = 0;

        if (driver_memory.number_steps) {
            --driver_memory.number_steps;

            SendPulse (TIM2, LL_TIM_CHANNEL_CH1, 3);
        } else {
            LL_TIM_DisableCounter (TIM2);
        }
    }
}