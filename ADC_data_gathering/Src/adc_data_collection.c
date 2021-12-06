/*
 * сбор данных с АЦП
 * фильтрация
 * вывод среднего значения
 */

#include "adc_data_collection.h"

ADC_HandleTypeDef ADC_Handle; /* ADC handle declaration */
ADC_ChannelConfTypeDef sADC_Config; /* ADC channel configuration structure declaration */
TIM_HandleTypeDef TimForADC_Handle; /*ADC timer handle*/

/* Variable containing ADC conversions data */
ALIGN_32BYTES(static uint16_t aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC1*/
ALIGN_32BYTES(static uint16_t aADC2ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC2*/
ALIGN_32BYTES(static uint16_t aADC3ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC3*/

// float32_t aIIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*TODO: IIR фильтр выкинуть или завести его выбор через макросы*/
float32_t aFIRfilterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
float32_t aADCvoltsData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /*массив с переведенными в вольты данными*/

void TIMforADC_Config(void)
{
//TODO	
}
