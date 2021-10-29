/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define A_CH2_FAULT_Pin GPIO_PIN_14
#define A_CH2_FAULT_GPIO_Port GPIOB
#define A_CH3_FAULT_Pin GPIO_PIN_15
#define A_CH3_FAULT_GPIO_Port GPIOB
#define A_CH1_FAULT_Pin GPIO_PIN_8
#define A_CH1_FAULT_GPIO_Port GPIOD
#define Button___Pin GPIO_PIN_2
#define Button___GPIO_Port GPIOG
#define Button__G3_Pin GPIO_PIN_3
#define Button__G3_GPIO_Port GPIOG
#define Button_Func_Pin GPIO_PIN_4
#define Button_Func_GPIO_Port GPIOG
#define Button_pH_Pin GPIO_PIN_5
#define Button_pH_GPIO_Port GPIOG
#define BUtton_Clock_Pin GPIO_PIN_6
#define BUtton_Clock_GPIO_Port GPIOG
#define Button_Emh_Pin GPIO_PIN_7
#define Button_Emh_GPIO_Port GPIOG
#define LEVEL_SWITCH_Pin GPIO_PIN_15
#define LEVEL_SWITCH_GPIO_Port GPIOA
#define SD_CARD_INS_Pin GPIO_PIN_3
#define SD_CARD_INS_GPIO_Port GPIOD
#define SD_CARD_CS_Pin GPIO_PIN_4
#define SD_CARD_CS_GPIO_Port GPIOD
#define SKIP_N_UNLOADED_REL_Pin GPIO_PIN_0
#define SKIP_N_UNLOADED_REL_GPIO_Port GPIOE
#define SKIP_N_UNLOADED_BEEPER_Pin GPIO_PIN_1
#define SKIP_N_UNLOADED_BEEPER_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
