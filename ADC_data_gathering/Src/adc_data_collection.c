/*
 * сбор данных с АЦП
 * фильтрация
 * вывод среднего значения
 */

#include "adc_data_collection.h"

/*Extern variables */
extern ADC_HandleTypeDef ADC1_Handle; /* ADC1 handle declaration */
extern ADC_HandleTypeDef ADC2_Handle; /* ADC1 handle declaration */
extern ADC_HandleTypeDef ADC3_Handle; /* ADC1 handle declaration */

void ADC_FilteredAverages(float32_t *AverageADC1, float32_t *AverageADC2, float32_t *AverageADC3)
{
	/* Variable containing ADC conversions data */
	ALIGN_32BYTES(volatile static uint16_t aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC1*/
	ALIGN_32BYTES(volatile static uint16_t aADC2ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC2*/
	ALIGN_32BYTES(volatile static uint16_t aADC3ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC3*/
	
	// float32_t aIIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*TODO: IIR фильтр выкинуть или завести его выбор через макросы*/
	static float32_t aFIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
	static float32_t aADCvoltsData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*массив с переведенными в вольты данными*/
	if (HAL_ADC_DeInit(&ADC1_Handle) != HAL_OK)
	{
		/* ADC de-initialization Error */
		Error_Handler();
	}
	
	/* ### - 4 - Start conversion in DMA mode ################################# */
	if (HAL_ADC_Start_DMA(&ADC1_Handle, (uint32_t*) aADC1ConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}
}

