/*
 * сбор данных с АЦП
 * фильтрация
 * вывод среднего значения
 */

#include "adc_data_collection.h"

/* Variable containing ADC conversions data */
ALIGN_32BYTES(static uint16_t aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC1*/
ALIGN_32BYTES(static uint16_t aADC2ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC2*/
ALIGN_32BYTES(static uint16_t aADC3ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC3*/

// float32_t aIIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*TODO: IIR фильтр выкинуть или завести его выбор через макросы*/
float32_t aFIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
float32_t aADCvoltsData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*массив с переведенными в вольты данными*/

if (HAL_ADC_DeInit(&ADC_Handle) != HAL_OK)
{	
	/* ADC de-initialization Error */
	Error_Handler();
}

/* ### - 4 - Start conversion in DMA mode ################################# */
if (HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*) aADC1ConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
{	
	Error_Handler();
}

void TIMforADC_Config(void)
{
//TODO	
}
