#include "StepMotorDriver.h"
#include "StepMotorParams.h"

#include "tim.h"
#include <stdint.h>
#include <strings.h>
#include <errno.h>

static sm_driver_memory_t drimem = {0}; // Driver memory  

  /// -------------------------------------------------------
  /* Эталон
  LL_TIM_CC_EnableChannel (TIM2, LL_TIM_CHANNEL_CH1);

  LL_TIM_OC_DisablePreload (TIM2, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetAutoReload (TIM2, 999);
  LL_TIM_OC_SetCompareCH1 (TIM2, 10);
  LL_TIM_OC_EnablePreload (TIM2, LL_TIM_CHANNEL_CH1);
  
  LL_TIM_OC_SetCompareCH1 (TIM2, 0);
  LL_TIM_EnableCounter (TIM2);
  */
  /// -------------------------------------------------------

__STATIC_FORCEINLINE void PrepearPulseChannel (timer_channel_t timer_channel, uint32_t pulse_len) {
    TIM_TypeDef* TIMx = timer_channel.TIMx;
    uint32_t TIM_CHANNEL_CHx = timer_channel.CHANNEL_CHx;

    LL_TIM_OC_DisablePreload (TIMx, TIM_CHANNEL_CHx);   // To instantly change timer settings
    LL_TIM_OC_SetCompareCH1 (TIMx, pulse_len);          // Set length pulse
    
    LL_TIM_OC_EnablePreload (TIMx, TIM_CHANNEL_CHx);    // So that after the pulse, the output is 0
    LL_TIM_OC_SetCompareCH1 (TIMx, 0);                  // Set 0 to CCRx
}

__STATIC_FORCEINLINE void SM_Prepear_Pulse_Channel (timer_channel_t timer_channel) {
    PrepearPulseChannel (timer_channel, PULSE_TIMER_COMPARE_VALUE);
}

__STATIC_FORCEINLINE void SendPulse (timer_channel_t timer_channel, uint32_t pulse_len) {
    PrepearPulseChannel (timer_channel, pulse_len);

    LL_TIM_EnableCounter (timer_channel.TIMx);                      // Start generate pulse
}

__STATIC_FORCEINLINE void SM_Send_Pulses (TIM_TypeDef* TIMx) {
    LL_TIM_EnableCounter (TIMx);                                    // Start generate pulses
}

__STATIC_INLINE void SM_Send_Step (timer_channel_t timer_channel) {
    SendPulse (timer_channel, PULSE_TIMER_COMPARE_VALUE);
}

void SM_Enable_TIM_Channel (timer_channel_t timer_channel) {
    LL_TIM_CC_EnableChannel (timer_channel.TIMx, timer_channel.CHANNEL_CHx);
}

void SM_Disable_TIM_Channel (timer_channel_t timer_channel) {
    LL_TIM_CC_DisableChannel (timer_channel.TIMx, timer_channel.CHANNEL_CHx);
}

#include "stm32f0xx.h"

// Только для одного таймера
void ST_Step_Driver () {
    static uint8_t channels_is_active = 0;
    if (channels_is_active) {
        LL_TIM_OC_DisablePreload (TIM2, LL_TIM_CHANNEL_CH1);
        LL_TIM_OC_DisablePreload (TIM2, LL_TIM_CHANNEL_CH2);
        
        for (int i = 0; i < NUMBER_STEP_MOTORS; ++i)
            LL_TIM_OC_SetCompareCH1 (drimem.step_motor[i].timer_channel.TIMx, 0);
        
        channels_is_active = 0;
    }
    
    for (uint32_t i = 0; i < NUMBER_STEP_MOTORS; ++i) {
        uint16_t* number_steps = &(drimem.unit_task[i].number_steps);

        if (*number_steps) {      
            sm_unit_task_t unit_task = drimem.unit_task[i];
            uint32_t counter = ++(drimem.counters[i]);

            if (counter == unit_task.timer_counter || counter == 0) {
                drimem.counters[i] = 0;
                
                (*number_steps)--;

                channels_is_active = 1;
                const timer_channel_t timer_channel = drimem.step_motor[i].timer_channel;

                SM_Prepear_Pulse_Channel (timer_channel);
            }
        }
    }
    
    //if (channels_is_active) 
    //    LL_TIM_EnableCounter (TIM2);
}

// Step motor driver memory ---------------------------------

void SM_Driver_Reset_Step_Motors () {
    bzero (&drimem.step_motor, sizeof (drimem.step_motor));
}
void SM_Driver_Reset_Unit_Tasks () {
    bzero (&drimem.unit_task, sizeof (drimem.unit_task));
}
void SM_Driver_Reset_Counters () {
    for (int i = 0; i < NUMBER_STEP_MOTORS; ++i)
        drimem.counters[i] = -1;
}
void SM_Driver_Reset_All () {
    SM_Driver_Reset_Step_Motors ();
    SM_Driver_Reset_Unit_Tasks  ();
    SM_Driver_Reset_Counters    ();
}

int SM_Driver_Init_Step_Motor (uint8_t number_step_motor, step_motor_t* step_motor) {
    if (number_step_motor >= NUMBER_STEP_MOTORS) {
        errno = ERANGE;
        return -1;
    } else if (step_motor == NULL) {
        errno = EINVAL;
        return -1;
    }

    drimem.step_motor[number_step_motor] = *step_motor;

    return 0;
}

int SM_Driver_Verifier () {
    for (int i = 0; i < NUMBER_STEP_MOTORS; ++i)
        if (SM_Step_Motor_Verificator (&drimem.step_motor[i]) == -1)
            return -1;

    return 0;
}

void SM_Driver_Set_Direction (uint16_t number_step_motor, uint8_t direction) {
    SM_Set_Direction (&drimem.step_motor[number_step_motor], direction);
}

void SM_Driver_Enable_Step_Motors () {
    SM_Enable (drimem.step_motor);
}
void SM_Driver_Disable_Step_Motors () {
    for (int i = 0; i < NUMBER_STEP_MOTORS; ++i)
        SM_Disable (&drimem.step_motor[i]);
}

void SM_Driver_Set_Task (uint16_t number_step_motor, const sm_unit_task_t* unit_task) {
    drimem.unit_task[number_step_motor] = *unit_task;

    SM_Driver_Set_Direction (number_step_motor, unit_task->direction);
    drimem.counters[number_step_motor] = -1;
}