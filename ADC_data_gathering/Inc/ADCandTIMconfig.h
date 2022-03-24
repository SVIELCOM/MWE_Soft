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
#include "config.h"

#define ADC_TRIGGER_FROM_TIMER

#define ADC_CONVERTED_DATA_BUFFER_SIZE ((uint32_t)  10000)		/* Size of array aADCxConvertedData[] must be 32 bytes aligned ,needed for cache maintenance purpose */

#if (ADC_bits==8U)
#define ADC_RANGE                   ((uint32_t)  255) 	/* Max digital value with a full range of 8 bits */
#define ADC_ZERO_OFFSET					((uint32_t)127)      /* Offset for zeroing ADC input*/
#elif (ADC_bits==10U)
#define ADC_RANGE                   ((uint32_t) 1023)    /* Max digital value with a full range of 12 bits */
#define ADC_ZERO_OFFSET					((uint32_t)511)      /* Offset for zeroing ADC input*/
#elif (ADC_bits==12U)
#define ADC_RANGE                   ((uint32_t) 4095)    /* Max digital value with a full range of 12 bits */
#define ADC_ZERO_OFFSET					((uint32_t)2047)     /* Offset for zeroing ADC input*/
#elif (ADC_bits==14U)
#define ADC_RANGE                   ((uint32_t)16383)    /* Max digital value with a full range of 14 bits */
#define ADC_ZERO_OFFSET					((uint32_t)8191)     /* Offset for zeroing ADC input*/
#else
#define ADC_RANGE                   ((uint32_t)65535)    /* Max digital value with a full range of 16 bits */
#define ADC_ZERO_OFFSET					((uint32_t)32767)    /* Offset for zeroing ADC input*/
#endif 																	/* ADC_bits */

/*Common ADC settins*/
/*Данные брать строго из дефайнов HAL */
#define ADC_CLOCK_SOURCE						RCC_ADCCLKSOURCE_PLL2
#define ADC_CLOCK_PRESCALER					ADC_CLOCK_ASYNC_DIV2	/* Asynchronous clock mode, input ADC clock divided by 2*/

#if 		(ADC_bits==8U)
#define 	ADC_RESOLUTION							ADC_RESOLUTION_8B		/* 8-bit resolution for converted data */
#elif 	(ADC_bits==10U)
#define 	ADC_RESOLUTION							ADC_RESOLUTION_10B	/* 10-bit resolution for converted data */
#elif 	(ADC_bits==12U)
#define 	ADC_RESOLUTION							ADC_RESOLUTION_12B	/* 12-bit resolution for converted data */
#elif 	(ADC_bits==14U)
#define 	ADC_RESOLUTION							ADC_RESOLUTION_14B	/* 14-bit resolution for converted data */
#else
#define 	ADC_RESOLUTION							ADC_RESOLUTION_16B	/* 16-bit resolution for converted data */
#endif

#define ADC_SAMPLING_TIME						ADC_SAMPLETIME_16CYCLES_5		//* sample time! It is not conversion time! */

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
#define ADC1_CHANNEL_GPIO_PORT          GPIOF
#else 
#define ADC1_CHANNEL_PIN                GPIO_PIN_1 								// for newer versions PCBs - ADC1 GPIO_PIN_11
#define ADC1_CHANNEL_GPIO_PORT          GPIOB
#endif

#define ADC2_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADC2_CHANNEL_PIN                GPIO_PIN_13 								// ADC2 GPIO pin number */
#define ADC2_CHANNEL_GPIO_PORT          GPIOF										

#define ADC3_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADC3_CHANNEL_PIN                GPIO_PIN_3 								// ADC3 GPIO pin number */
#define ADC3_CHANNEL_GPIO_PORT          GPIOC										

/* Definition for ADCx's Channels */
#if defined (PCB_VERSION_0_1)
#define ADC1_CHANNEL                    ADC_CHANNEL_6 							//* for version PCB-0.1 - ADC1 CHANNEL 6*/
#else 
#define ADC1_CHANNEL                    ADC_CHANNEL_5 							// for newer versions PCBs - ADC1 CHANNEL 2
#endif
#define ADC2_CHANNEL                    ADC_CHANNEL_2 
#define ADC3_CHANNEL                    ADC_CHANNEL_1 

/* Definition of ADCx DMA resources */
#define ADC1_DMA_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE()			//*  WARNING! */
#define ADC1_DMA                        DMA2_Stream4
#define ADC1_DMA_IRQn                   DMA2_Stream4_IRQn
#define ADC1_DMA_IRQHandler             DMA2_STR4_IRQHandler

#define ADC2_DMA_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE() 			//*  WARNING! */
#define ADC2_DMA                        DMA2_Stream5						
#define ADC2_DMA_IRQn                   DMA2_Stream5_IRQn				
#define ADC2_DMA_IRQHandler             DMA2_STR5_IRQHandler		

#define ADC3_DMA_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE()			//*  WARNING! */
#define ADC3_DMA                        DMA2_Stream6						
#define ADC3_DMA_IRQn                   DMA2_Stream6_IRQn				
#define ADC3_DMA_IRQHandler             DMA2_STR6_IRQHandler		

#define ADC_DMA_PRIORITY					 DMA_PRIORITY_MEDIUM						//* DMA transfer for ADC priority */
#define ADC_DMA_FIFOMODE					 DMA_FIFOMODE_DISABLE	
#define ADC_DMA_FIFOLEVEL					 DMA_FIFO_THRESHOLD_FULL				//* DMA transfer for ADC fifo level*/

/* Definition of ADCx NVIC resources */
#define ADC12_IRQn                       ADC_IRQn
#define ADC12_IRQHandler                 ADC_IRQHandler

/*Definition of ADC trigger*/
#define ADC_EXTERNALTRIGCONV_Tx_TRGO    ADC_EXTERNALTRIG_T15_TRGO			

#if  defined (ADC_TRIGGER_FROM_TIMER)
/* Timer for ADC trigger parameters */
/* ## Definition of TIM for ADC related resources ################################### */
/* Definition of TIMx clock resources */
#define TIM_FOR_ADC                     TIM15   							 		/* Может быть только TIM1,TIM8 или 15 (только они могут выдать заданное количество тактов). Все они на AHB2 и APB2 */
#define TIM_FOR_ADC_CLK_ENABLE()               __HAL_RCC_TIM15_CLK_ENABLE() 		/* при замене таймера, все эти параметры надо менять на свои от каждого таймера*/
#define TIM_FOR_ADC_FORCE_RESET()              __HAL_RCC_TIM15_FORCE_RESET()
#define TIM_FOR_ADC_RELEASE_RESET()            __HAL_RCC_TIM15_RELEASE_RESET()
#define TIM_FOR_ADC_FREQUENCY                ((uint32_t) 	2000)    				/* Timer frequency (unit: Hz). With a timer 16 bits and time base freq min 1Hz, range is min=1Hz, max=32kHz. */
#define TIM_FOR_ADC_FREQUENCY_RANGE_MIN      ((uint32_t)    1)    					/* Timer minimum frequency (unit: Hz), used to calculate frequency range. With a timer 16 bits, maximum frequency will be 32000 times this value. */
#define TIM_FOR_ADC_PRESCALER_MAX_VALUE      (0xFFFF-1)           					/* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */
#define TIM_FOR_ADC_NB_PULSES						(ADC_CONVERTED_DATA_BUFFER_SIZE-1)	/* Number of timer pulses */

#endif /* ADC_TRIGGER_FROM_TIMER */

/*Function declarations*/
void Error_Handler(void);
void TIM_for_ADC_Config(TIM_TypeDef *timer);
void ADC_Config(ADC_HandleTypeDef *ADC_Handle, uint32_t ADC_channel);

#endif /* ADCANDTIMCONFIG_H_ */
