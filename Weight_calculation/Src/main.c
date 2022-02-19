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
void PeriphCommonClock_Config(void);
// static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	uint8_t path_switch_togles = 0;
	/* Enable the CPU Cache */
	CPU_CACHE_Enable();
	HAL_Init();
	
	/* Configure the system clock to */
	SystemClock_Config();
	PeriphCommonClock_Config();
	pathSwitchConfig();
	
	/* an example to use the function */
//	weightFormula_t weight_handler; /* create a variable of weightFormula type */
	weightFormulaIntInp_t massint_handler;
	
	/* коэффициенты для подсчета с использованием int */
	float F1 = 0.0590953006632f; /* fill the coefficients */
	float F2L = 0.00206542f;     //42.007494109f;
	float F2R = 0.00215521f;   	//244.0f;
	float F3L = 251.0f;
	float F3R = 657.202f;
	
	/* коэффициенты для подсчета с использованием float */
	float dF1 = 0.0590953006632f; /* fill the coefficients */
	float dF2L = 41.0120656556887f;
	float dF2R = 42.7176212461233f;
	float dF3L = 0.0f;
	float dF3R = 0.0f;
	
	double massfromaverage, averagemass;
	extern volatile uint16_t aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
	extern volatile uint16_t aADC2ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
	extern volatile uint16_t aADC3ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
	int_fast64_t ZeroOffsetMotorVoltage = 32442;
	int_fast64_t ZeroOffsetMotorCurrent = 32605;
	int_fast64_t ZeroOffsetMotorSpeed = 32587;
	
	// double motorVoltage, motorCurrent, motorSpeed;     //временные переменные пока не разберусь с кастованием указателя float32 в double или выкину функцию
	//uint16_t fakeADCconvertedData[1] = { 30000 };
	pathSwitchItEnable();
	while (1)
	{
		if (pathSwitchToggled)
		{
			pathSwitchItDisable();
			pathSwitchToggled = 0;
			path_switch_togles++;
			
			StartADCdataCollection();
		}
		if (GetAllFreshAnalogChannelsValues(ADC_CONVERTED_DATA_BUFFER_SIZE) == SUCCESS)
		{
			/*
			 weight_handler.coefficientF1 = &F1;  fill the structure 
			 if (pathSwitchPos == GPIO_PIN_SET)
			 {
			 weight_handler.coefficientF2 = &F2L;
			 weight_handler.coefficientF3 = &F3L;
			 } else
			 {
			 weight_handler.coefficientF2 = &F2R;
			 weight_handler.coefficientF3 = &F3R;
			 }
			 
			 motorVoltage = (double) AnalogCH1_collected_data;
			 motorCurrent = (double) AnalogCH2_collected_data;
			 motorSpeed = (double) AnalogCH3_collected_data;
			 weight_handler.motorVoltage = &motorVoltage;
			 weight_handler.motorCurrent = &motorCurrent;
			 weight_handler.motorSpeed = &motorSpeed;
			 getSkipWeight(&weight_handler);
			 massfromaverage = weight_handler.result;
			 */
			if (pathSwitchPos == GPIO_PIN_SET)
			{
				
				averagemass = get_average_mass(aADC1ConvertedData, aADC2ConvertedData, aADC3ConvertedData, ADC_RANGE, ADC_CONVERTED_DATA_BUFFER_SIZE, &dF1, &dF2L, &dF3L, &ZeroOffsetMotorVoltage,
						&ZeroOffsetMotorCurrent, &ZeroOffsetMotorSpeed);
			} else
			{
				
				averagemass = get_average_mass(aADC1ConvertedData, aADC2ConvertedData, aADC3ConvertedData, ADC_RANGE, ADC_CONVERTED_DATA_BUFFER_SIZE, &dF1, &dF2R, &dF3R, &ZeroOffsetMotorVoltage,
						&ZeroOffsetMotorCurrent, &ZeroOffsetMotorSpeed);
			}
			if (pathSwitchPos == GPIO_PIN_SET)
			{
				massint_handler.coefficientF2 = &F2L;
				massint_handler.coefficientF3 = &F3L;
			} else
			{
				massint_handler.coefficientF2 = &F2R;
				massint_handler.coefficientF3 = &F3R;
			}
			massint_handler.coefficientF1 = &F1;
			massint_handler.motorVoltage = aADC1ConvertedData;
			massint_handler.motorCurrent = aADC2ConvertedData;
			massint_handler.motorSpeed = aADC3ConvertedData;
			massint_handler.intMultiplier = 100000;
			massint_handler.ZeroOffsetMotorVoltage = &ZeroOffsetMotorVoltage;
			massint_handler.ZeroOffsetMotorCurrent = &ZeroOffsetMotorCurrent;
			massint_handler.ZeroOffsetMotorSpeed = &ZeroOffsetMotorSpeed;
			get_average_mass_int(&massint_handler, ADC_CONVERTED_DATA_BUFFER_SIZE);
			pathSwitchItEnable();
		}
		(void) averagemass;
		(void) massfromaverage;
	}
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
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);
	
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

void PeriphCommonClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	
	/** Initializes the peripherals clock
	 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInitStruct.PLL2.PLL2M = 4;
	PeriphClkInitStruct.PLL2.PLL2N = 90;
	PeriphClkInitStruct.PLL2.PLL2P = 3;
	PeriphClkInitStruct.PLL2.PLL2Q = 2;
	PeriphClkInitStruct.PLL2.PLL2R = 2;
	PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_1;
	PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
	PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
	PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
/*
 static void Error_Handler(void)
 {
 
 //	BSP_LED_On(LED3);
 while (1)
 {
 }
 }
 */

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

