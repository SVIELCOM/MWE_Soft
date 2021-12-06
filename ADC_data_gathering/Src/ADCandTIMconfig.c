/*
 * ADCandTIMconfig.c
 *
 *  Created on: 6 дек. 2021 г.
 *      Author: cableman
 */

#include "ADCandTIMconfig.h"
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_conf.h"

ADC_HandleTypeDef ADC_Handle; /* ADC handle declaration */
ADC_ChannelConfTypeDef sADC_Config; /* ADC channel configuration structure declaration */
TIM_HandleTypeDef TimForADC_Handle; /*ADC timer handle*/

/* ### - 1 - Initialize ADC1 peripheral #################################### */
ADC_Handle.Instance = ADC_1;
if (HAL_ADC_DeInit(&ADC_Handle) != HAL_OK)
{	
	/* ADC de-initialization Error */
	Error_Handler();
}

ADC_Handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2; /* Asynchronous clock mode, input ADC clock divided by 2*/
ADC_Handle.Init.Resolution = ADC_RESOLUTION_16B; /* 16-bit resolution for converted data */
ADC_Handle.Init.ScanConvMode = DISABLE; /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
ADC_Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV; /* EOC flag picked-up to indicate conversion end */
ADC_Handle.Init.LowPowerAutoWait = DISABLE; /* Auto-delayed conversion feature disabled */
ADC_Handle.Init.ContinuousConvMode = ENABLE; /* Continuous mode enabled (automatic conversion restart after each conversion) */
ADC_Handle.Init.NbrOfConversion = 1; /* Parameter discarded because sequencer is disabled */
ADC_Handle.Init.DiscontinuousConvMode = DISABLE; /* Parameter discarded because sequencer is disabled */
ADC_Handle.Init.NbrOfDiscConversion = 1; /* Parameter discarded because sequencer is disabled */
ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START; /* Software start to trig the 1st conversion manually, without external event */
ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
ADC_Handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; /* ADC DMA circular requested */
ADC_Handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; /* DR register is overwritten with the last conversion result in case of overrun */

/* Test with OVERSAMPLING ENABLED*/
ADC_Handle.Init.OversamplingMode = DISABLE; /* No oversampling */
ADC_Handle.Init.Oversampling.Ratio = 16;
ADC_Handle.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_6;
ADC_Handle.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
ADC_Handle.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;

/* Initialize ADC peripheral according to the passed parameters */
if (HAL_ADC_Init(&ADC_Handle) != HAL_OK)
{	
	Error_Handler();
}

/* ### - 2 - Start calibration ############################################ */
if (HAL_ADCEx_Calibration_Start(&ADC_Handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
{	
	Error_Handler();
}

/* ### - 3 - Channel configuration ######################################## */
sADC_Config.Channel = ADCx_CHANNEL; /* Sampled channel number */
sADC_Config.Rank = ADC_REGULAR_RANK_1; /* Rank of sampled channel number ADCx_CHANNEL */
sADC_Config.SamplingTime = ADC_SAMPLETIME_8CYCLES_5; /* Sampling time (number of clock cycles unit) */
sADC_Config.SingleDiff = ADC_SINGLE_ENDED; /* Single-ended input channel */
sADC_Config.OffsetNumber = ADC_OFFSET_NONE; /* No offset subtraction */
sADC_Config.Offset = 0; /* Parameter discarded because offset correction is disabled */
if (HAL_ADC_ConfigChannel(&ADC_Handle, &sADC_Config) != HAL_OK)
{	
	Error_Handler();
}

/* ### - 4 - Start conversion in DMA mode ################################# */
if (HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*) aADC1ConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
{	
	Error_Handler();
}

void DMA1_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(ADC_Handle.DMA_Handle);
}
