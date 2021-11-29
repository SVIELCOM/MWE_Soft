/**
 ******************************************************************************
 * @file    ADC/ADC_DMA_Transfer/Src/main.c
 * @author  MCD Application Team
 * @brief   This example describes how to use the DMA to convert continuously data.
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

#define __FPU_PRESENT 	1

#include "main.h"
#include "arm_math.h"

/** @addtogroup STM32H7xx_HAL_Examples
 * @{
 */

/** @addtogroup ADC_DMA_Transfer
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Definitions of data related to this example */
/* Definition of ADCx conversions data table size */
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  32)   /* Size of array aADCxConvertedData[] */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handle declaration */
ADC_HandleTypeDef AdcHandle;

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef sConfig;

/* Variable containing ADC conversions data */
ALIGN_32BYTES(static uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]);

float32_t aIIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
float32_t aFIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
float32_t aADCvoltsData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*массив с переведенными в вольты данными*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	/* This sample code shows how to convert an analog input and read the converted
	 data using DMA transfer.
	 To proceed, 4 steps are required: */

	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	/* STM32H7xx HAL library initialization:
	 - Systick timer is configured by default as source of time base, but user
	 can eventually implement his proper time base source (a general purpose
	 timer for example or other time source), keeping in mind that Time base
	 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	 handled in milliseconds basis.
	 - Set NVIC Group Priority to 4
	 - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the system clock to 400 MHz */
	SystemClock_Config();

	/* Initialize LED3 */
	BSP_LED_Init(LED3);

	/* ### - 1 - Initialize ADC peripheral #################################### */
	AdcHandle.Instance = ADCx;
	if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
	{
		/* ADC de-initialization Error */
		Error_Handler();
	}

	AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2; /* Asynchronous clock mode, input ADC clock divided by 2*/
	AdcHandle.Init.Resolution = ADC_RESOLUTION_16B; /* 16-bit resolution for converted data */
	AdcHandle.Init.ScanConvMode = DISABLE; /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV; /* EOC flag picked-up to indicate conversion end */
	AdcHandle.Init.LowPowerAutoWait = DISABLE; /* Auto-delayed conversion feature disabled */
	AdcHandle.Init.ContinuousConvMode = ENABLE; /* Continuous mode enabled (automatic conversion restart after each conversion) */
	AdcHandle.Init.NbrOfConversion = 1; /* Parameter discarded because sequencer is disabled */
	AdcHandle.Init.DiscontinuousConvMode = DISABLE; /* Parameter discarded because sequencer is disabled */
	AdcHandle.Init.NbrOfDiscConversion = 1; /* Parameter discarded because sequencer is disabled */
	AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START; /* Software start to trig the 1st conversion manually, without external event */
	AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
	AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; /* ADC DMA circular requested */
	AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; /* DR register is overwritten with the last conversion result in case of overrun */

	/* Test with OVERSAMPLING ENABLED*/
	AdcHandle.Init.OversamplingMode = DISABLE; /* No oversampling */
	AdcHandle.Init.Oversampling.Ratio = 16;
	AdcHandle.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_6;
	AdcHandle.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	AdcHandle.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;

	/* Initialize ADC peripheral according to the passed parameters */
	if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
	{
		Error_Handler();
	}

	/* ### - 2 - Start calibration ############################################ */
	if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}

	/* ### - 3 - Channel configuration ######################################## */
	sConfig.Channel = ADCx_CHANNEL; /* Sampled channel number */
	sConfig.Rank = ADC_REGULAR_RANK_1; /* Rank of sampled channel number ADCx_CHANNEL */
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5; /* Sampling time (number of clock cycles unit) */
	sConfig.SingleDiff = ADC_SINGLE_ENDED; /* Single-ended input channel */
	sConfig.OffsetNumber = ADC_OFFSET_NONE; /* No offset subtraction */
	sConfig.Offset = 0; /* Parameter discarded because offset correction is disabled */
	if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/* ### - 4 - Start conversion in DMA mode ################################# */
	if (HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*) aADCxConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}

	/******************Замутил тут шляпу для проверки фильтрации тупым усреднением и фильтром IIR *************/
	/*настройка фильтра хорошо описана тут https://schaumont.dyn.wpi.edu/ece4703b21/lecture7.html#:~:text=considerable%20performance%20improvement.-,IIR%20Designs%20using%20ARM%20CMSIS%20DSP,-Just%20as%20with*/

	/*данные для IIR фильтра*/
#define IIR_FILTER_NUM_STAGES	2  /* количество секций 2го порядка в фильтре*/
	float64_t IIRfilterCoefficients[IIR_FILTER_NUM_STAGES * 5] = { 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579, 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579 };
	float64_t IIRfilter_taps[4 * IIR_FILTER_NUM_STAGES];

	/*данные для FIR фильтра*/
#define FIR_FILTER_NUM_STAGES	21  /* количество секций фильтра*/

	/*коэффициенты фильтра*/
	float32_t FIRfilterCoefficients[FIR_FILTER_NUM_STAGES] = { 0.0072524808347225189208984375, 0.009322776459157466888427734375, 0.01530767977237701416015625, 0.02464949898421764373779296875,
			0.0364511311054229736328125, 0.04956446588039398193359375, 0.062704540789127349853515625, 0.07457792758941650390625, 0.084012426435947418212890625, 0.0900747776031494140625,
			0.09216459095478057861328125, 0.0900747776031494140625, 0.084012426435947418212890625, 0.07457792758941650390625, 0.062704540789127349853515625, 0.04956446588039398193359375,
			0.0364511311054229736328125, 0.02464949898421764373779296875, 0.01530767977237701416015625, 0.009322776459157466888427734375, 0.0072524808347225189208984375 };

	float32_t FIRfilter_taps[FIR_FILTER_NUM_STAGES + ADC_CONVERTED_DATA_BUFFER_SIZE - 1];

	arm_biquad_casd_df1_inst_f32 IIRfilterInstance; /*структура для работы функции*/
	arm_fir_instance_f32 FIRfilterInstance; /*структура для работы функции*/

	static float32_t VoltADCCoeffitient; /*TODO: потом переделать на вычисление из заданного количества бит дискретизации*/
	VoltADCCoeffitient = 3.3f / 65536;

	float32_t VoltsAverage, IIRVoltsAverage, FIRVoltsAverage;
	VoltsAverage = 0.0f; /* переменные для усреднения */

	/*вызов инициализации фильтров*/
	arm_fir_init_f32(&FIRfilterInstance, FIR_FILTER_NUM_STAGES, FIRfilterCoefficients, FIRfilter_taps, ADC_CONVERTED_DATA_BUFFER_SIZE);
	arm_biquad_cascade_df1_init_f32(&IIRfilterInstance, IIR_FILTER_NUM_STAGES, IIRfilterCoefficients, IIRfilter_taps);

	/* Infinite Loop */
	while (1)
	{
		for (uint8_t indx = 0; indx < ADC_CONVERTED_DATA_BUFFER_SIZE; indx++) /*сформируем массив, в котором будут данные в вольтах*/
		{
			aADCvoltsData[indx] = (float32_t) aADCxConvertedData[indx] * VoltADCCoeffitient;
		}
		/* считаем тупое среднее*/
		float32_t summa = 0;
		for (uint8_t indx = 0; indx < ADC_CONVERTED_DATA_BUFFER_SIZE; indx++)
		{
			summa += aADCvoltsData[indx];
		}
		VoltsAverage = (VoltsAverage + (float) summa) / (ADC_CONVERTED_DATA_BUFFER_SIZE + 1);
		/************************/

		/* закидывавем массив в фильтры*/
		arm_biquad_cascade_df1_f32(&IIRfilterInstance, aADCvoltsData, aIIRfilterConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE);
		arm_fir_f32(&FIRfilterInstance, aADCvoltsData, aFIRfilterConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE);

		float32_t summaVolts = 0; /*если потом использовать, то убрать эту и использовать одну переменную для суммы*/
		for (uint8_t indx = 0; indx < ADC_CONVERTED_DATA_BUFFER_SIZE; indx++)
		{
			summaVolts += aIIRfilterConvertedData[indx];
		}
		IIRVoltsAverage = (IIRVoltsAverage + (float) summaVolts) / (ADC_CONVERTED_DATA_BUFFER_SIZE + 1);

		summaVolts = 0;
		for (uint8_t indx = 0; indx < ADC_CONVERTED_DATA_BUFFER_SIZE; indx++)
		{
			summaVolts += aFIRfilterConvertedData[indx];
		}
		FIRVoltsAverage = (FIRVoltsAverage + (float) summaVolts) / (ADC_CONVERTED_DATA_BUFFER_SIZE + 1);

	}
	/**********************************************************************************************************/
	(void) aADCvoltsData; /*чтобы не ругался на неиспользуемость переменной*/
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE BYPASS)
 *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
 *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
 *            AHB Prescaler                  = 2
 *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
 *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
 *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
 *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 4
 *            PLL_N                          = 400
 *            PLL_P                          = 2
 *            PLL_Q                          = 4
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Flash Latency(WS)              = 4
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/*!< Supply configuration update enable */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
	{
	}

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLQ = 4;

	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if (ret != HAL_OK)
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure  bus clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 |
	RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
	if (ret != HAL_OK)
	{
		Error_Handler();
	}

}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED3 on */
	BSP_LED_On(LED3);
	while (1)
	{
	}
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

/**
 * @brief  Conversion complete callback in non-blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes */
	SCB_InvalidateDCache_by_Addr((uint32_t*) &aADCxConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
}

/**
 * @brief  Conversion DMA half-transfer callback in non-blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
	SCB_InvalidateDCache_by_Addr((uint32_t*) &aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE / 2], ADC_CONVERTED_DATA_BUFFER_SIZE);
}
/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
