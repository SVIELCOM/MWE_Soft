/**
 ******************************************************************************
 * @file    ADC/ADC_DMA_Transfer/Src/stm32h7xx_it.c
 * @author  MCD Application Team
 * @brief   Main Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ADCandTIMconfig.h"
#include "stm32h7xx_it.h"

extern ADC_HandleTypeDef ADC1_Handle; /* ADC1 handle declaration */
extern ADC_HandleTypeDef ADC2_Handle; /* ADC1 handle declaration */
extern ADC_HandleTypeDef ADC3_Handle; /* ADC1 handle declaration */

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	HAL_IncTick();
}

/** Следить за тем, чтобы эти хендлеры в стартапе назывались так же! */

void DMA2_STR4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(ADC1_Handle.DMA_Handle);
}

void DMA2_STR5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(ADC2_Handle.DMA_Handle);
}

void DMA2_STR6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(ADC3_Handle.DMA_Handle);
}
