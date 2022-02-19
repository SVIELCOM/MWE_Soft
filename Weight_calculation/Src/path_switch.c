/*
 * path_switch.c
 *
 *  Created on: 18 янв. 2022 г.
 *      Author: cableman
 */

#include "path_switch.h"

void pathSwitchConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	PATH_SWITCH_GPIO_CLK_ENABLE();
	/* Configure GPIO and PIN as input */
	GPIO_InitStructure.Mode = PATH_SWITCH_PIN_MODE;
	GPIO_InitStructure.Pull = PATH_SWITCH_PIN_PULL_MODE;
	GPIO_InitStructure.Pin = PATH_SWITCH_PIN;
	HAL_GPIO_Init(PATH_SWITCH_GPIO_PORT, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(PATH_SWITCH_IT_NUMBER, PATH_SWITCH_IT_PRIO, 0);
}

void pathSwitchItEnable(void)
{
	HAL_NVIC_EnableIRQ(PATH_SWITCH_IT_NUMBER);     //возможно, стоит переписать функции, чтобы не отключать все прерывания 10-15, 
																  //а поднимать/опускать флаг
}

void pathSwitchItDisable(void)
{
	HAL_NVIC_DisableIRQ(PATH_SWITCH_IT_NUMBER);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(PATH_SWITCH_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == PATH_SWITCH_PIN)
	{
		pathSwitchToggled = 1;
		pathSwitchPos = HAL_GPIO_ReadPin(PATH_SWITCH_GPIO_PORT, PATH_SWITCH_PIN);
	}
}
