/*
 * path_switch.h
 *
 *  Created on: 18 янв. 2022 г.
 *      Author: cableman
 */

#ifndef PATH_SWITCH_H_
#define PATH_SWITCH_H_

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_conf.h"

/* Path switch recourses */
#define PATH_SWITCH_PIN						GPIO_PIN_15
#define PATH_SWITCH_GPIO_PORT				GPIOA
#define PATH_SWITCH_GPIO_CLK_ENABLE() 	__HAL_RCC_GPIOA_CLK_ENABLE()
#define PATH_SWITCH_PIN_MODE				GPIO_MODE_IT_RISING_FALLING
#define PATH_SWITCH_PIN_PULL_MODE		GPIO_NOPULL
#define PATH_SWITCH_IT_NUMBER				EXTI15_10_IRQn
#define PATH_SWITCH_IT_PRIO				2

/* export variables */
uint8_t pathSwitchToggled;
uint8_t pathSwitchPos;

/* functions declaration */
void pathSwitchConfig(void);
void pathSwitchItEnable(void);
void pathSwitchItDisable(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);     //TODO: выпилить отсюда этот колбэк, куда - пока не решил, либо расширить функционал
																	 //этой библиотеки для обработки не только прерывания от этажного выключателя, но 
																	 //и от всех кнопок
void EXTI4_IRQHandler(void);
#endif /* PATH_SWITCH_H_ */
