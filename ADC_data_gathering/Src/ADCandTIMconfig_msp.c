/* Includes ------------------------------------------------------------------*/
#include "ADCandTIMconfig.h"

/**
 * @brief  ADC MSP Init
 * @param  hadc : ADC handle
 * @retval None
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	static DMA_HandleTypeDef DmaHandle;
	
	/* common DMA settings */
	DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
	DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	DmaHandle.Init.Mode = DMA_CIRCULAR;
	DmaHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
	
	if (hadc->Instance == ADC1)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/

		ADC1_CHANNEL_PIN_CLK_ENABLE();   			  //* Enable GPIO clock ****************************************/
		ADC1_CLK_ENABLE();     								//* ADC Periph clock enable */
		__HAL_RCC_ADC_CONFIG(ADC_CLOCK_SOURCE);     //* ADC Periph interface clock configuration */
		ADC1_DMA_CLK_ENABLE();     						//* Enable DMA clock */
		GPIO_InitStruct.Pin = ADC1_CHANNEL_PIN;		//* ADC Channel GPIO pin configuration */
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADC1_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
		DmaHandle.Instance = ADC1_DMA;
		DmaHandle.Init.Request = DMA_REQUEST_ADC1;
		
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle);
		HAL_DMA_Init(&DmaHandle);
		
		/* Associate the DMA handle */
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
		
		/* NVIC configuration for DMA Input data interrupt */
		HAL_NVIC_SetPriority(ADC1_DMA_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(ADC1_DMA_IRQn);
		
	} else if (hadc->Instance == ADC2)
	{
		ADC2_CHANNEL_PIN_CLK_ENABLE();     				//* Enable GPIO clock ****************************************/
		ADC2_CLK_ENABLE();     								//* ADC Periph clock enable */
		__HAL_RCC_ADC_CONFIG(ADC_CLOCK_SOURCE);     //* ADC Periph interface clock configuration */
		ADC2_DMA_CLK_ENABLE();     						//* Enable DMA clock */
		GPIO_InitStruct.Pin = ADC2_CHANNEL_PIN;		//* ADC Channel GPIO pin configuration */
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADC2_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
		DmaHandle.Instance = ADC2_DMA;
		DmaHandle.Init.Request = DMA_REQUEST_ADC2;
		
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle);
		HAL_DMA_Init(&DmaHandle);
		
		/* Associate the DMA handle */
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
		
		/* NVIC configuration for DMA Input data interrupt */
		HAL_NVIC_SetPriority(ADC2_DMA_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(ADC2_DMA_IRQn);
	} else if (hadc->Instance == ADC3)
	{
		ADC3_CHANNEL_PIN_CLK_ENABLE();     				//* Enable GPIO clock ****************************************/
		ADC3_CLK_ENABLE();     								//* ADC Periph clock enable */
		__HAL_RCC_ADC_CONFIG(ADC_CLOCK_SOURCE);     	//* ADC Periph interface clock configuration */
		ADC3_DMA_CLK_ENABLE();								//* Enable DMA clock */
		GPIO_InitStruct.Pin = ADC3_CHANNEL_PIN;		//* ADC Channel GPIO pin configuration */
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADC3_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
		DmaHandle.Instance = ADC3_DMA;
		DmaHandle.Init.Request = DMA_REQUEST_ADC3;
		
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle);
		HAL_DMA_Init(&DmaHandle);
		
		/* Associate the DMA handle */
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
		
		/* NVIC configuration for DMA Input data interrupt */
		HAL_NVIC_SetPriority(ADC3_DMA_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(ADC3_DMA_IRQn);
	}
	
	/*##- 2- Configure peripheral GPIO #########################################*/
	/* ADC Channel GPIO pin configuration */

	/*##- 3- Configure DMA #####################################################*/
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	/* TIM peripheral clock enable */
	if (htim->Instance == TIM_FOR_ADC)
	{
		TIM_FOR_ADC_CLK_ENABLE();
	} else
	{
		/* Error management can be implemented here */
	}
	
}

/****
 * Если нужна нога, на которой будут импульсы таймера для АЦП, то настроить!
 * правльность GPIO не проверялась!
 * @param htim
 */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (htim->Instance == TIM_FOR_ADC)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
		/**TIM15 GPIO Configuration
		 PE5     ------> TIM15_CH1
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF4_TIM15;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		
	}
}

/**
 * @brief TIM MSP de-initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable clock of peripheral
 * @param htim: TIM handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
	/* TIM peripheral clock reset */
	if (htim->Instance == TIM_FOR_ADC)
	{
		TIM_FOR_ADC_FORCE_RESET();
		TIM_FOR_ADC_RELEASE_RESET();
	} else
	{
		/* Error management can be implemented here */
	}
}

/**
 * @brief ADC MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO to their default state
 *          - Функция деинициализирует все ADC и отключит все порты GPIO, где были ноги ADC1,2,3
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1)
	{
		ADC1_FORCE_RESET();
		ADC1_RELEASE_RESET();
		__HAL_RCC_ADC12_CLK_DISABLE();
		HAL_GPIO_DeInit(ADC1_CHANNEL_GPIO_PORT, ADC1_CHANNEL_PIN);
	} else if (hadc->Instance == ADC1)
	{
		ADC2_FORCE_RESET();
		ADC2_RELEASE_RESET();
		__HAL_RCC_ADC12_CLK_DISABLE();
		HAL_GPIO_DeInit(ADC2_CHANNEL_GPIO_PORT, ADC2_CHANNEL_PIN);
	} else if (hadc->Instance == ADC1)
	{
		ADC3_FORCE_RESET();
		ADC3_RELEASE_RESET();
		__HAL_RCC_ADC3_CLK_DISABLE();
		HAL_GPIO_DeInit(ADC3_CHANNEL_GPIO_PORT, ADC3_CHANNEL_PIN);
	}
}
