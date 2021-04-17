#include "StepMotorParams.h"
#include "StepMotorDriver.h"
#include "main.h"

int InitializeDriverStepMotors () {
    SM_Driver_Reset_All ();

    step_motor_t step_motor_0 = {
        .timer_channel = {
            .TIMx        = SM_PULSE_TIMER,
            .CHANNEL_CHx = SM_0_TIM_CHANNEL
        },
        .enable = {
            .pin  = ENABLE_Pin,
            .port = ENABLE_GPIO_Port
        },
        .dir = {
            .pin  = DIR_0_Pin,
            .port = DIR_0_GPIO_Port
        }
    };

    step_motor_t step_motor_1 = {
        .timer_channel = {
            .TIMx        = SM_PULSE_TIMER,
            .CHANNEL_CHx = SM_1_TIM_CHANNEL
        },
        .enable = {
            .pin  = ENABLE_Pin,
            .port = ENABLE_GPIO_Port
        },
        .dir = {
            .pin  = DIR_1_Pin,
            .port = DIR_1_GPIO_Port
        }
    };

    step_motor_t step_motor_2 = {
        .timer_channel = {
            .TIMx        = SM_PULSE_TIMER,
            .CHANNEL_CHx = SM_2_TIM_CHANNEL
        },
        .enable = {
            .pin  = ENABLE_Pin,
            .port = ENABLE_GPIO_Port
        },
        .dir = {
            .pin  = DIR_2_Pin,
            .port = DIR_2_GPIO_Port 
        }
    };

    if (SM_Driver_Init_Step_Motor (0, &step_motor_0) == -1 ||
        SM_Driver_Init_Step_Motor (1, &step_motor_1) == -1 ||
        SM_Driver_Init_Step_Motor (2, &step_motor_2) == -1)
        return -1;

    if (SM_Driver_Verifier () == -1)
        return -1;

    SM_Disable_TIM_Channel ((timer_channel_t) {
        .TIMx = TIM2,
        .CHANNEL_CHx = SM_0_TIM_CHANNEL
    });

    SM_Disable_TIM_Channel ((timer_channel_t) {
        .TIMx = TIM2,
        .CHANNEL_CHx = SM_1_TIM_CHANNEL
    });

    SM_Disable_TIM_Channel ((timer_channel_t) {
        .TIMx = TIM2,
        .CHANNEL_CHx = SM_2_TIM_CHANNEL
    });

    LL_TIM_DisableCounter (TIM2);

    return 0;
}