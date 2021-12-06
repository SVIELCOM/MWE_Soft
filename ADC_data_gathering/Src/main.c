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

#include "main.h"

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
	
	/* Enable the CPU Cache */
//	CPU_CACHE_Enable();
	HAL_Init();
	
	/* Configure the system clock to 400 MHz */
	SystemClock_Config();
	
	/* Initialize LED3 */
	//BSP_LED_Init(LED3);
	TIM_Config();
	
	/******************Замутил тут шляпу для проверки фильтрации тупым усреднением и фильтром IIR *************/
	/*настройка фильтра хорошо описана тут https://schaumont.dyn.wpi.edu/ece4703b21/lecture7.html#:~:text=considerable%20performance%20improvement.-,IIR%20Designs%20using%20ARM%20CMSIS%20DSP,-Just%20as%20with*/

	/*данные для IIR фильтра*/
	/*
	 #define IIR_FILTER_NUM_STAGES	2  // количество секций 2го порядка в фильтре
	 float64_t IIRfilterCoefficients[IIR_FILTER_NUM_STAGES * 5] = { 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579, 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579 };
	 float64_t IIRfilter_taps[4 * IIR_FILTER_NUM_STAGES];
	 */
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
	
	float32_t VoltsAverage, FIRVoltsAverage; /* IIRVoltsAverage - removed */
	VoltsAverage = 0.0f; /* переменные для усреднения */
	
	/*вызов инициализации фильтров*/
	arm_fir_init_f32(&FIRfilterInstance, FIR_FILTER_NUM_STAGES, FIRfilterCoefficients, FIRfilter_taps, ADC_CONVERTED_DATA_BUFFER_SIZE);
//	arm_biquad_cascade_df1_init_f32(&IIRfilterInstance, IIR_FILTER_NUM_STAGES, IIRfilterCoefficients, IIRfilter_taps);
	
	/* Infinite Loop */
	while (1)
	{
		for (uint8_t indx = 0; indx < ADC_CONVERTED_DATA_BUFFER_SIZE; indx++) /*сформируем массив, в котором будут данные в вольтах*/
		{
			aADCvoltsData[indx] = (float32_t) aADC1ConvertedData[indx] * VoltADCCoeffitient;
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
		// arm_biquad_cascade_df1_f32(&IIRfilterInstance, aADCvoltsData, aIIRfilterConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE);
		arm_fir_f32(&FIRfilterInstance, aADCvoltsData, aFIRfilterConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE);
		
		float32_t summaVolts = 0; /*если потом использовать, то убрать эту и использовать одну переменную для суммы*/
//		for (uint8_t indx = 0; indx < ADC_CONVERTED_DATA_BUFFER_SIZE; indx++)
//		{
//			summaVolts += aIIRfilterConvertedData[indx];
//		}
//		IIRVoltsAverage = (IIRVoltsAverage + (float) summaVolts) / (ADC_CONVERTED_DATA_BUFFER_SIZE + 1);
		
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
//	BSP_LED_On(LED3);
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
	SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC1ConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
}

/**
 * @brief  Conversion DMA half-transfer callback in non-blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
	SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE / 2], ADC_CONVERTED_DATA_BUFFER_SIZE);
}
/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
