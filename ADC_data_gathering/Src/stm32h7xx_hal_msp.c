/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
	
	if (hadc = ADC1)
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
		DmaHandle.Instance = DMA1_Stream1;
		DmaHandle.Init.Request = DMA_REQUEST_ADC1;
		
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle);
		HAL_DMA_Init(&DmaHandle);
		
		/* Associate the DMA handle */
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
		
		/* NVIC configuration for DMA Input data interrupt */
		HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
		
	} else if (hadc = ADC2)
	{
		ADC2_CHANNEL_PIN_CLK_ENABLE();     				//* Enable GPIO clock ****************************************/
		ADC2_CLK_ENABLE();     								//* ADC Periph clock enable */
		__HAL_RCC_ADC_CONFIG(ADC_CLOCK_SOURCE);     //* ADC Periph interface clock configuration */
		ADC2_DMA_CLK_ENABLE();     						//* Enable DMA clock */
		GPIO_InitStruct.Pin = ADC2_CHANNEL_PIN;		//* ADC Channel GPIO pin configuration */
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADC2_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
		DmaHandle.Instance = DMA1_Stream1; 					//* TODO  выставить правильное значение*/
		DmaHandle.Init.Request = DMA_REQUEST_ADC1; 		//* TODO  выставить правильное значение*/
				
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle);
		HAL_DMA_Init(&DmaHandle);
		
		/* Associate the DMA handle */
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
		
		/* NVIC configuration for DMA Input data interrupt */
		HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);     //* TODO  выставить правильное значение*/
		HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);				//* TODO  выставить правильное значение*/
	} else if (hadc = ADC3)
	{
		ADC3_CHANNEL_PIN_CLK_ENABLE();     				//* Enable GPIO clock ****************************************/
		ADC3_CLK_ENABLE();     								//* ADC Periph clock enable */
		__HAL_RCC_ADC_CONFIG(ADC_CLOCK_SOURCE);     	//* ADC Periph interface clock configuration */
		ADC3_DMA_CLK_ENABLE();								//* Enable DMA clock */
		GPIO_InitStruct.Pin = ADC3_CHANNEL_PIN;		//* ADC Channel GPIO pin configuration */
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADC3_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
		DmaHandle.Instance = DMA1_Stream1; 					//* TODO  выставить правильное значение*/
		DmaHandle.Init.Request = DMA_REQUEST_ADC1; 		//* TODO  выставить правильное значение*/
				
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle);
		HAL_DMA_Init(&DmaHandle);
		
		/* Associate the DMA handle */
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);
		
		/* NVIC configuration for DMA Input data interrupt */
		HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);     //* TODO  выставить правильное значение*/
		HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);				//* TODO  выставить правильное значение*/
	}
	
	/*##- 2- Configure peripheral GPIO #########################################*/
	/* ADC Channel GPIO pin configuration */

	/*##- 3- Configure DMA #####################################################*/
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
	/*##-1- Reset peripherals ##################################################*/
	ADC1_FORCE_RESET();
	ADC2_FORCE_RESET();
	ADC3_FORCE_RESET()
	ADC1_RELEASE_RESET();
	ADC2_RELEASE_RESET();
	ADC3_RELEASE_RESET();
	/* ADC Periph clock disable
	 (automatically reset all ADC instances of the ADC common group) */
	__HAL_RCC_ADC12_CLK_DISABLE();
	
	/*##-2- Disable peripherals and GPIO Clocks ################################*/
	/* De-initialize the ADC Channel GPIO pin */
	HAL_GPIO_DeInit(ADC1_CHANNEL_GPIO_PORT, ADC1_CHANNEL_PIN);
	HAL_GPIO_DeInit(ADC2_CHANNEL_GPIO_PORT, ADC2_CHANNEL_PIN);
	HAL_GPIO_DeInit(ADC3_CHANNEL_GPIO_PORT, ADC3_CHANNEL_PIN);
}
