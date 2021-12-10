/*
 * ADCandTIMconfig.c
 *
 *  Created on: 6 дек. 2021 г.
 *      Author: cableman
 */

#include "ADCandTIMconfig.h"

ADC_HandleTypeDef ADC1_Handle; 			//* ADC1 handle declaration */
ADC_HandleTypeDef ADC2_Handle; 			//* ADC2 handle declaration */
ADC_HandleTypeDef ADC3_Handle; 			//* ADC3 handle declaration */
DMA_HandleTypeDef ADC1_DMA_Handle;		//* ADC1 handle declaration */
DMA_HandleTypeDef ADC2_DMA_Handle;		//* ADC2 handle declaration */
DMA_HandleTypeDef ADC3_DMA_Handle;		//* ADC3 handle declaration */

TIM_HandleTypeDef TimForADC_Handle; /*TIM Handle for triggering ADC declaration*/

/* ### - Initialize ADC peripheral #################################### */
/**
 * 
 * @param ADC_Handle - указатель на хэндлер АЦП
 * @param ADC_channel - номер канала АЦП
 */
void ADC_Config(ADC_HandleTypeDef *ADC_Handle, uint32_t ADC_channel)
{
	ADC_ChannelConfTypeDef sADC_Config; /* ADC channel configuration structure declaration */
	if (HAL_ADC_DeInit(ADC_Handle) != HAL_OK)
	{
		/* ADC de-initialization Error */
		Error_Handler();
	}
	ADC_Handle->Init.ClockPrescaler = ADC_CLOCK_PRESCALER;
	ADC_Handle->Init.Resolution = ADC_RESOLUTION;
	ADC_Handle->Init.ScanConvMode = DISABLE; /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	ADC_Handle->Init.EOCSelection = ADC_EOC_SINGLE_CONV; /* EOC flag picked-up to indicate conversion end */
	ADC_Handle->Init.LowPowerAutoWait = DISABLE; /* Auto-delayed conversion feature disabled */
#if defined (ADC_TRIGGER_FROM_TIMER)
	ADC_Handle->Init.ContinuousConvMode = DISABLE; /* Continuous mode disabled to have only 1 conversion at each conversion trig */
	ADC_Handle->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Tx_TRGO;
	ADC_Handle->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	
#else
	ADC1_Handle.Init.ContinuousConvMode = ENABLE; /* Continuous mode enabled (automatic conversion restart after each conversion) */
	ADC1_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START; /* Software start to trig the 1st conversion manually, without external event */
	ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
#endif
	
	ADC_Handle->Init.NbrOfConversion = 1; /* Parameter discarded because sequencer is disabled */
	ADC_Handle->Init.DiscontinuousConvMode = DISABLE; /* Parameter discarded because sequencer is disabled */
	ADC_Handle->Init.NbrOfDiscConversion = 1; /* Parameter discarded because sequencer is disabled */
	
	ADC_Handle->Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; /* ADC DMA circular requested */
	ADC_Handle->Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; /* DR register is overwritten with the last conversion result in case of overrun */
	
#if defined (ADC_OVERSAMPLING)
	/* OVERSAMPLING section*/
	ADC1_Handle.Init.OversamplingMode = DISABLE; /* No oversampling */
	ADC1_Handle.Init.Oversampling.Ratio = 16;
	ADC1_Handle.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_6;
	ADC1_Handle.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	ADC1_Handle.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
#endif
	
	/* Initialize ADC peripheral according to the passed parameters */
	if (HAL_ADC_Init(ADC_Handle) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* Start calibration ############################################ */
	if (HAL_ADCEx_Calibration_Start(ADC_Handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* ### - Channel configuration ######################################## */
	sADC_Config.Channel = ADC_channel; /* Sampled channel number */
	sADC_Config.Rank = ADC_REGULAR_RANK_1; /* Rank of sampled channel number ADCx_CHANNEL */
	sADC_Config.SamplingTime = ADC_SAMPLING_TIME; /* Sampling time (number of clock cycles unit) */
	sADC_Config.SingleDiff = ADC_SINGLE_ENDED; /* Single-ended input channel */
	sADC_Config.OffsetNumber = ADC_OFFSET_NONE; /* No offset subtraction */
	sADC_Config.Offset = 0; /* Parameter discarded because offset correction is disabled */
	if (HAL_ADC_ConfigChannel(ADC_Handle, &sADC_Config) != HAL_OK)
	{
		Error_Handler();
	}
}

#if defined (ADC_TRIGGER_FROM_TIMER)

void TIM_for_ADC_Config(TIM_TypeDef *timer)
{
	
	TimForADC_Handle.Instance = timer; /* Set timer instance */
	TIM_MasterConfigTypeDef master_timer_config;
	RCC_ClkInitTypeDef clk_init_struct = { 0 };/* Temporary variable to retrieve RCC clock configuration */
	TIM_OC_InitTypeDef sConfigOC = { 0 };
//	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	uint32_t latency;/* Temporary variable to retrieve Flash Latency */
	
	uint32_t timer_clock_frequency = 0;/* Timer clock frequency */
	uint32_t timer_prescaler = 0;/* Time base prescaler to have timebase aligned on minimum frequency possible */
	
	/* Configuration of timer as time base:                                     */
	/* Caution: Computation of frequency is done for a timer instance on APB1   */
	/*          (clocked by PCLK1)                                              */
	/* Timer frequency is configured from the following constants:              */
	/* - TIMER_FREQUENCY: timer frequency (unit: Hz).                           */
	/* - TIMER_FREQUENCY_RANGE_MIN: timer minimum frequency possible            */
	/*   (unit: Hz).                                                            */
	/* Note: Refer to comments at these literals definition for more details.   */

	/* Retrieve timer clock source frequency */
	HAL_RCC_GetClockConfig(&clk_init_struct, &latency);
	/* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
	/* clock source.                                                            */
	if (clk_init_struct.APB2CLKDivider == RCC_HCLK_DIV1)
	{
		timer_clock_frequency = HAL_RCC_GetPCLK2Freq();
	} else
	{
		timer_clock_frequency = HAL_RCC_GetPCLK2Freq() * 2;
	}
	
	/* Timer prescaler calculation */
	/* (computation for timer 16 bits, additional + 1 to round the prescaler up) */
	timer_prescaler = (timer_clock_frequency / (TIM_FOR_ADC_PRESCALER_MAX_VALUE * TIM_FOR_ADC_FREQUENCY_RANGE_MIN)) + 1;
	
	/**** TODO: проверить правильность формирования частоты импульсов таймера */

	/* Configure timer parameters */
	TimForADC_Handle.Init.Period = ((timer_clock_frequency / (timer_prescaler * TIM_FOR_ADC_FREQUENCY)) - 1);
	TimForADC_Handle.Init.Prescaler = (timer_prescaler - 1);
	TimForADC_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TimForADC_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimForADC_Handle.Init.RepetitionCounter = TIM_FOR_ADC_NB_PULSES;
	TimForADC_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	if (HAL_TIM_Base_Init(&TimForADC_Handle) != HAL_OK)
	{
		/* Timer initialization Error */
		Error_Handler();
	}
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&TimForADC_Handle, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_TIM_OC_Init(&TimForADC_Handle) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_TIM_OnePulse_Init(&TimForADC_Handle, TIM_OPMODE_SINGLE) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* Timer TRGO selection */
	master_timer_config.MasterOutputTrigger = TIM_TRGO_OC1;							//* Это триггер для старта ADC. Формируется, когда срабатывает сравнение */
	master_timer_config.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	master_timer_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	if (HAL_TIMEx_MasterConfigSynchronization(&TimForADC_Handle, &master_timer_config) != HAL_OK)
	{
		/* Timer TRGO selection Error */
		Error_Handler();
	}
	
	sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_OC_ConfigChannel(&TimForADC_Handle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* Так как таймер не использует режим прерывания счета по внешнему сигналу, то можно это выпилить или запилить прерывание
	 sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	 sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	 sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	 sBreakDeadTimeConfig.DeadTime = 0;
	 sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	 sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	 sBreakDeadTimeConfig.BreakFilter = 0;
	 sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	 if (HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sBreakDeadTimeConfig) != HAL_OK)
	 {
	 Error_Handler();
	 }

	 */

//	HAL_TIM_MspPostInit(&TimForADC_Handle);
}
#endif

void Error_Handler(void)
{
	/* Turn LED3 on */
//	BSP_LED_On(LED3);
	while (1)
	{
	}
}

