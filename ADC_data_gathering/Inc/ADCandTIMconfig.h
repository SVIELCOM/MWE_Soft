/*
 * ADCandTIMconfig.h
 *
 *  Created on: 6 дек. 2021 г.
 *      Author: cableman
 */

#ifndef ADCANDTIMCONFIG_H_
#define ADCANDTIMCONFIG_H_

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_conf.h"

/*Common ADC settins*/
/*Данные брать строго из дефайнов HAL */
#define ADC_CLOCK_SOURCE						RCC_ADCCLKSOURCE_CLKP
#define ADC_CLOCK_PRESCALER					ADC_CLOCK_ASYNC_DIV2	/* Asynchronous clock mode, input ADC clock divided by 2*/
#define ADC_RESOLUTION							ADC_RESOLUTION_16B	/* 16-bit resolution for converted data */
#define ADC_SAMPLING_TIME						ADC_SAMPLETIME_8CYCLES_5

/* Definition for ADC1 clock resources */

/* ADC1 */
#define ADC_1                            ADC1
#define ADC1_CLK_ENABLE()               __HAL_RCC_ADC12_CLK_ENABLE()
#define ADC1_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()			//* GPIOF для порта ADC1*/

#define ADC1_DMAx_CHANNELx_CLK_ENABLE()      __HAL_RCC_DMA2_CLK_ENABLE()	//* using DMA2 for ADC2*/

#define ADC1_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADC1_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* ADC2 */
#define ADC_2                            	ADC2
#define ADC2_CLK_ENABLE()               	__HAL_RCC_ADC12_CLK_ENABLE()
#define ADC2_CHANNEL_GPIO_CLK_ENABLE()  	__HAL_RCC_GPIOF_CLK_ENABLE() 		//* GPIOF для порта ADC2*/

#define ADC2_DMAx_CHANNELx_CLK_ENABLE()   __HAL_RCC_DMA2_CLK_ENABLE() 		//* using DMA2 for ADC2*/

#define ADC2_FORCE_RESET()              	__HAL_RCC_ADC12_FORCE_RESET() 
#define ADC2_RELEASE_RESET()            	__HAL_RCC_ADC12_RELEASE_RESET()

/* ADC3 */
#define ADC_3                            	ADC3
#define ADC3_CLK_ENABLE()               	__HAL_RCC_ADC3_CLK_ENABLE()
#define ADC3_CHANNEL_GPIO_CLK_ENABLE()  	__HAL_RCC_GPIOC_CLK_ENABLE() 		//* GPIOC for ADC2*/

#define ADC3_DMAx_CHANNELx_CLK_ENABLE()   __HAL_RCC_DMA2_CLK_ENABLE() 		//* using DMA2 for ADC3*/

#define ADC3_FORCE_RESET()              	__HAL_RCC_ADC3_FORCE_RESET()
#define ADC3_RELEASE_RESET()            	__HAL_RCC_ADC3_RELEASE_RESET()

/* Definition for ADCx Channel Pins */
#define ADC1_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#if defined (PCB_VERSION_0_1)
#define ADC1_CHANNEL_PIN                GPIO_PIN_12 								// for version PCB-0.1 - ADC1 GPIO_PIN_12
#else 
#define ADC1_CHANNEL_PIN                GPIO_PIN_11 								// for newer versions PCBs - ADC1 GPIO_PIN_11
#endif
#define ADC1_CHANNEL_GPIO_PORT          GPIOF

#define ADC2_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADC2_CHANNEL_PIN                GPIO_PIN_13 								// ADC2 GPIO pin number */
#define ADC2_CHANNEL_GPIO_PORT          GPIOF										

#define ADC3_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADC3_CHANNEL_PIN                GPIO_PIN_12 								// ADC3 GPIO pin number */
#define ADC3_CHANNEL_GPIO_PORT          GPIOC										

/* Definition for ADCx's Channels */
#if defined (PCB_VERSION_0_1)
#define ADC1_CHANNEL                    ADC_CHANNEL_6 							//* for version PCB-0.1 - ADC1 CHANNEL 6*/
#else 
#define ADC1_CHANNEL                    ADC_CHANNEL_2 							// for newer versions PCBs - ADC1 CHANNEL 2
#endif
#define ADC2_CHANNEL                    ADC_CHANNEL_2 
#define ADC3_CHANNEL                    ADC_CHANNEL_1 

/* Definition of ADCx DMA resources */
#define ADC1_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define ADC1_DMA                        DMA2_Stream4
#define ADC1_DMA_IRQn                   DMA2_Stream4_IRQn
#define ADC1_DMA_IRQHandler             DMA2_Stream4_IRQHandler

#define ADC2_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE() 
#define ADC2_DMA                        DMA2_Stream5						
#define ADC2_DMA_IRQn                   DMA2_Stream5_IRQn				
#define ADC2_DMA_IRQHandler             DMA2_Stream5_IRQHandler		

#define ADC3_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()	
#define ADC3_DMA                        DMA2_Stream6						
#define ADC3_DMA_IRQn                   DMA2_Stream6_IRQn				
#define ADC3_DMA_IRQHandler             DMA2_Stream6_IRQHandler		

/* Definition of ADCx NVIC resources */
#define ADC12_IRQn                       ADC_IRQn
#define ADC12_IRQHandler                 ADC_IRQHandler

/*Definition of ADC trigger*/
#define ADC_EXTERNALTRIGCONV_Tx_TRGO    ADC_EXTERNALTRIG_T15_TRGO			

#if  defined (ADC_TRIGGER_FROM_TIMER)
/* Timer for ADC trigger parameters */
/* ## Definition of TIM for ADC related resources ################################### */
/* Definition of TIMx clock resources */
#define TIM_FOR_ADC                     TIM15   							 	/* Может быть только TIM1,TIM8 или 15 (только они могут выдать заданное количество тактов). Все они на AHB2 и APB2 */
#define TIMx_CLK_ENABLE()               __HAL_RCC_TIM15_CLK_ENABLE() 	/* при замене таймера, все эти параметры надо менять на свои от каждого таймера*/
#define TIMx_FORCE_RESET()              __HAL_RCC_TIM15_FORCE_RESET()
#define TIMx_RELEASE_RESET()            __HAL_RCC_TIM15_RELEASE_RESET()
#define TIMER_FREQUENCY                ((uint32_t) 	2000)    				/* Timer frequency (unit: Hz). With a timer 16 bits and time base freq min 1Hz, range is min=1Hz, max=32kHz. */
#define TIMER_FREQUENCY_RANGE_MIN      ((uint32_t)    1)    				/* Timer minimum frequency (unit: Hz), used to calculate frequency range. With a timer 16 bits, maximum frequency will be 32000 times this value. */
#define TIMER_PRESCALER_MAX_VALUE      (0xFFFF-1)           				/* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */

#endif /* ADC_TRIGGER_FROM_TIMER */

/*fuctions declarations*/
void TIMforADC_Config(void);

#endif /* ADCANDTIMCONFIG_H_ */
