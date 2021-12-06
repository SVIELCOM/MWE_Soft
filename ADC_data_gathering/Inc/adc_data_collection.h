/*
 * adc_data_collection.h
 *
 *  Created on: Dec 3, 2021
 *      Author: cableman
 */

#ifndef ADC_DATA_COLLECTION_H_
#define ADC_DATA_COLLECTION_H_

#include "arm_math.h"
#include "ADCandTIMconfig.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_conf.h"

#define __FPU_PRESENT 	1

#define ADC_TRIGGER_FROM_TIMER

/* TODO: задействовать использование этих макросов, чтобы иметь возможность переключать разрешение АЦП */
#define RANGE_8BITS                    ((uint32_t)  255) 	/* Max digital value with a full range of 8 bits */
#define RANGE_10BITS                   ((uint32_t) 1023)    /* Max digital value with a full range of 12 bits */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max digital value with a full range of 12 bits */
#define RANGE_14BITS                   ((uint32_t)16383)    /* Max digital value with a full range of 14 bits */
#define RANGE_16BITS                   ((uint32_t)65535)    /* Max digital value with a full range of 16 bits */

#define ADC_CONVERTED_DATA_BUFFER_SIZE ((uint32_t)  32)		/* Size of array aADCxConvertedData[] */

/* Trigger for ADC:                                                           */
/*  - If this literal is defined: ADC is operating in not continuous mode     */
/*    and conversions are trigger by external trigger: timer.                 */
/*  - If this literal is not defined: ADC is operating in continuous mode     */
/*    and first conversion is triggered by software trigger.                  */

/* Definition for ADC1 clock resources */

/* ADC1 */
#define ADC_1                            ADC1
#define ADC1_CLK_ENABLE()               __HAL_RCC_ADC12_CLK_ENABLE()
#define ADC1_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

#define ADC1_DMAx_CHANNELx_CLK_ENABLE()      __HAL_RCC_DMA1_CLK_ENABLE()

#define ADC1_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADC1_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* ADC2 */
#define ADC_2                            	ADC2
#define ADC2_CLK_ENABLE()               	__HAL_RCC_ADC12_CLK_ENABLE()
#define ADC2_CHANNEL_GPIO_CLK_ENABLE()  	__HAL_RCC_GPIOF_CLK_ENABLE()

#define ADC2_DMAx_CHANNELx_CLK_ENABLE()   __HAL_RCC_DMA1_CLK_ENABLE()

#define ADC2_FORCE_RESET()              	__HAL_RCC_ADC12_FORCE_RESET()
#define ADC2_RELEASE_RESET()            	__HAL_RCC_ADC12_RELEASE_RESET()

/* ADC3 */
#define ADC_3                            	ADC3
#define ADC3_CLK_ENABLE()               	__HAL_RCC_ADC12_CLK_ENABLE()
#define ADC3_CHANNEL_GPIO_CLK_ENABLE()  	__HAL_RCC_GPIOF_CLK_ENABLE()

#define ADC3_DMAx_CHANNELx_CLK_ENABLE()   __HAL_RCC_DMA1_CLK_ENABLE()

#define ADC3_FORCE_RESET()              	__HAL_RCC_ADC12_FORCE_RESET()
#define ADC3_RELEASE_RESET()            	__HAL_RCC_ADC12_RELEASE_RESET()

/* Definition for ADCx Channel Pins */
#define ADC1_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADC1_CHANNEL_PIN                GPIO_PIN_12 		// для платы весов GPIO_PIN_12
#define ADC1_CHANNEL_GPIO_PORT          GPIOF

#define ADC2_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADC2_CHANNEL_PIN                GPIO_PIN_12 		/* TODO  выставить правильный номер!*/
#define ADC2_CHANNEL_GPIO_PORT          GPIOF				/* TODO  выставить правильный номер!*/

#define ADC3_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define ADC3_CHANNEL_PIN                GPIO_PIN_12 		/* TODO  выставить правильный номер!*/
#define ADC3_CHANNEL_GPIO_PORT          GPIOF				/* TODO  выставить правильный номер!*/

/* Definition for ADCx's Channels */
#define ADC1_CHANNEL                    ADC_CHANNEL_6 /* номер канала для ADC1 в новой версии платы будет другой! Уточнить!*/
#define ADC2_CHANNEL                    ADC_CHANNEL_6 /* TODO  выставить правильный номер!*/
#define ADC3_CHANNEL                    ADC_CHANNEL_6 /* TODO  выставить правильный номер!*/

/********************* еще не введено в эксплуатацию*******************/
/* Definition of ADCx DMA resources */
#define ADCx_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx_DMA                        DMA1_Stream1

#define ADCx_DMA_IRQn                   DMA1_Stream1_IRQn
#define ADCx_DMA_IRQHandler             DMA1_Stream1_IRQHandler

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                       ADC_IRQn
#define ADCx_IRQHandler                 ADC_IRQHandler
/************************************************************************/

#if  defined (ADC_TRIGGER_FROM_TIMER)
/* Timer for ADC trigger parameters */
/* ## Definition of TIM for ADC related resources ################################### */
/* Definition of TIMx clock resources */
#define TIMx                            TIM15   							 	/* Может быть только TIM1,TIM8 или 15 (только они могут выдать заданное количество тактов). Все они на AHB2 и APB2 */
#define TIMx_CLK_ENABLE()               __HAL_RCC_TIM15_CLK_ENABLE() 	/* при замене таймера, все эти параметры надо менять на свои от каждого таймера*/
#define TIMx_FORCE_RESET()              __HAL_RCC_TIM15_FORCE_RESET()
#define TIMx_RELEASE_RESET()            __HAL_RCC_TIM15_RELEASE_RESET()
#define ADC_EXTERNALTRIGCONV_Tx_TRGO    ADC_EXTERNALTRIG_T15_TRGO			/*триггер для запуска ADC*/
#define TIMER_FREQUENCY                ((uint32_t) 	100)    				/* Timer frequency (unit: Hz). With a timer 16 bits and time base freq min 1Hz, range is min=1Hz, max=32kHz. */
#define TIMER_FREQUENCY_RANGE_MIN      ((uint32_t)    1)    				/* Timer minimum frequency (unit: Hz), used to calculate frequency range. With a timer 16 bits, maximum frequency will be 32000 times this value. */
#define TIMER_PRESCALER_MAX_VALUE      (0xFFFF-1)           				/* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */
#endif /* ADC_TRIGGER_FROM_TIMER */

/*fuctions declarations*/
void TIMforADC_Config(void);

#endif /* ADC_DATA_COLLECTION_H_ */
