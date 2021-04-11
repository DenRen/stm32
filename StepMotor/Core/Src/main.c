/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "StepMotorLib.h"
#include "StepMotorParams.h"
#include "StepMotorDriver.h"
#include "axis.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int step = 0, durum = 0, sayac = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/*void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1) // eger kesme kaynagi timer1 den gelmis ise
  {
    sayac++;           // her kesmede sayaci arttir.
    if (sayac == step) // eger sayac istenilen adim sayisi kadar artmis ise
    {
      HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1); // PWM'i durdur.
      sayac = 0;                                  // bir sonraki komutta sayac sifirdan baslamali
      durum = 0;                                  // step fonksiyonunun i�indeki while d�ng�s�nden �ikilmasi i�in
                                                  // durum degiskeni sifirlandi.
    }
  }
}

void Step (int adim, int yon) // step motor fonksiyonu. iki adet parametre alicak. Pals ve y�n parametreleri
{
  step = adim;
  if (yon == 0)
    HAL_GPIO_WritePin (DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET); // eger y�n bilgisi sifir ise DIR pini lojik 0 yapildi.
  else
    HAL_GPIO_WritePin (DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET); // sifirdan farkli bir deger ise DIR pini lojik 1 yapildi.
  
  durum = 1;
  HAL_TIM_PWM_Start_IT (&htim1, TIM_CHANNEL_1); // PWM'i baslatildi.
  while (1)                                    // step motorun istenilen konuma gitmesi i�in belli bir s�re gececek
  {                                            //	bu s�re boyunca programin tekrar step fonksiyonunun i�ine girmesini �nlemek i�in sonsuz d�ng� olusturuldu.
    if (durum == 0)                            // step motor istenilen konuma gittiginde durum degiskeni sifir olacak ve d�ng�den �ikilacak.
      break;
    
    HAL_Delay(1);
  }
}*/
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void ErrorActions () {
  LL_GPIO_ResetOutputPin (LD3_GPIO_Port, LD3_Pin);
  LL_GPIO_ResetOutputPin (LD4_GPIO_Port, LD4_Pin);

  while (1) {  
    LL_GPIO_SetOutputPin (LD3_GPIO_Port, LD3_Pin);
    LL_GPIO_SetOutputPin (LD4_GPIO_Port, LD4_Pin);
    LL_mDelay (200);

    LL_GPIO_ResetOutputPin (LD3_GPIO_Port, LD3_Pin);
    LL_GPIO_ResetOutputPin (LD4_GPIO_Port, LD4_Pin);
    LL_mDelay (200);

  }
}

void PrepareTimes () {
  LL_TIM_CC_EnableChannel (TIM2, LL_TIM_CHANNEL_CH1);

  LL_TIM_DisableCounter (SM_DRIVER_TIMER);
  LL_TIM_DisableCounter (TIM2);
  
  LL_TIM_ClearFlag_UPDATE (SM_DRIVER_TIMER);
  LL_TIM_ClearFlag_UPDATE (TIM2);

  LL_TIM_EnableIT_UPDATE (SM_DRIVER_TIMER);
  LL_TIM_EnableIT_UPDATE (TIM2);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  if (InitializeDriverStepMotors () == -1)
    ErrorActions ();

  for (int i = 0; i < 3; ++i) {
    LL_mDelay (30);
    LL_GPIO_SetOutputPin (LD4_GPIO_Port, LD4_Pin);  
    LL_mDelay (30);
    LL_GPIO_ResetOutputPin (LD4_GPIO_Port, LD4_Pin);
  }

  PrepareTimes ();
  SM_Driver_Enable_Step_Motors ();  

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
    LL_GPIO_SetOutputPin (LD4_GPIO_Port, LD4_Pin);
    LL_mDelay (250);

    AxisRotate (0, 2 * PI_URAD, 1000 * 400);
    
    LL_GPIO_ResetOutputPin (LD4_GPIO_Port, LD4_Pin);
    LL_mDelay (250);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(48000000);
  LL_SetSystemCoreClock(48000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
