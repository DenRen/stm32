#include "StepMotorParams.h"
#include "StepMotorDriver.h"
#include "main.h"

int InitializeDriverStepMotors () {
    SM_Driver_Reset_All ();

    step_motor_t step_motor = {
        .timer_channel = {
            .TIMx        = SM_PULSE_TIMER_1,
            .CHANNEL_CHx = SM_1_TIM_CHANNEL
        },
        .enable = {
            .pin  = ENABLE_Pin,
            .port = ENABLE_GPIO_Port
        },
        .dir = {
            .pin  = DIR_Pin,
            .port = DIR_GPIO_Port
        }
    };

    if (SM_Driver_Init_Step_Motor (0, &step_motor) == -1)
        return -1;

    if (SM_Driver_Verifier () == -1)
        return -1;

    return 0;
}