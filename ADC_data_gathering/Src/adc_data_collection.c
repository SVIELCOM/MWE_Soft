/*
 * сбор данных с АЦП
 * фильтрация
 * вывод среднего значения
 */

#include "adc_data_collection.h"

/*Extern variables */
extern ADC_HandleTypeDef ADC1_Handle; /* ADC1 handle declaration */
extern ADC_HandleTypeDef ADC2_Handle; /* ADC2 handle declaration */
extern ADC_HandleTypeDef ADC3_Handle; /* ADC3 handle declaration */
extern TIM_HandleTypeDef TimForADC_Handle; /*TIM Handle for triggering ADC extern declaration*/
/* static Functions*/
static void DataFiltering(float32_t *pSrc, float32_t *pDst, uint32_t buffer_size);
static float32_t AverageCalc(float32_t *AverageVolts, uint32_t buffer_size);
static float32_t AverageCalc(float32_t *pSrc, uint32_t buffer_size);
static void ADCdata_to_volts(uint32_t ADC_max_value, volatile uint16_t *pSrc, float32_t *pDst, uint32_t size_of_data_array);
static float32_t GetAnalogChannelValue(volatile uint16_t *ACDxconvertedData, uint32_t ADC_Range, uint32_t buffer_size);

/* Variable containing ADC conversions data. Aligned 32-bytes,needed for cache maintenance purpose */
ALIGN_32BYTES(volatile static uint16_t aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC1 */
ALIGN_32BYTES(volatile static uint16_t aADC2ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC2*/
ALIGN_32BYTES(volatile static uint16_t aADC3ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]); /* data from ADC3*/

/* Static variables */
/*коэффициенты фильтра*/
float64_t FIRfilterCoefficients[FIR_FILTER_NUM_STAGES] = { 0.0072524808347225189208984375, 0.009322776459157466888427734375, 0.01530767977237701416015625, 0.02464949898421764373779296875,
		0.0364511311054229736328125, 0.04956446588039398193359375, 0.062704540789127349853515625, 0.07457792758941650390625, 0.084012426435947418212890625, 0.0900747776031494140625,
		0.09216459095478057861328125, 0.0900747776031494140625, 0.084012426435947418212890625, 0.07457792758941650390625, 0.062704540789127349853515625, 0.04956446588039398193359375,
		0.0364511311054229736328125, 0.02464949898421764373779296875, 0.01530767977237701416015625, 0.009322776459157466888427734375, 0.0072524808347225189208984375 };

void StartADCdataCollection(void)
{
	/* ADC Handle intialization*/
	ADC1_Handle.Instance = ADC_1;
	ADC2_Handle.Instance = ADC_2;
	ADC3_Handle.Instance = ADC_3;
	
	ADC_Config(&ADC1_Handle, ADC1_CHANNEL);
	ADC_Config(&ADC2_Handle, ADC2_CHANNEL);
	ADC_Config(&ADC3_Handle, ADC3_CHANNEL);
	
	TIM_for_ADC_Config(TIM_FOR_ADC);
	
	/* Start conversion in DMA mode */

	if (HAL_ADC_Start_DMA(&ADC1_Handle, (uint32_t*) aADC1ConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_ADC_Start_DMA(&ADC2_Handle, (uint32_t*) aADC2ConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_ADC_Start_DMA(&ADC3_Handle, (uint32_t*) aADC3ConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}
	
	ADC_ConvCplt = 0x0; /* reset flag "ADCs conversions complete" */
#if defined (ADC_TRIGGER_FROM_TIMER)
	/*Start TIMER*/
	//HAL_TIM_OC_Start(&TimForADC_Handle, TIM_CHANNEL_1); /* Для режима, в котором таймер останавливается по RCR */
	if (HAL_TIM_Base_Start(&TimForADC_Handle) != HAL_OK)
	{
		Error_Handler();
	}
#endif
}

/*******************************
 * 
 * @param ADC_Range					- наибольшее число, которое отдаст АЦП (16bit - 65535)
 * @param buffer_size				- размер буфера данных
 * @return								- возвращаем SUCCESS, если только все АЦП закончили преобразование
 */
uint8_t GetAllFreshAnalogChannelsValues(uint32_t buffer_size)
{
	if (ADC_ConvCplt != 0x7)
		return UNSUCCESS;
	else									//* возвращаем только когда все каналы закончили преобразование */
	{
		AnalogCH1_collected_data = GetAnalogChannelValue(aADC1ConvertedData, ADC_RANGE, buffer_size);
		AnalogCH2_collected_data = GetAnalogChannelValue(aADC2ConvertedData, ADC_RANGE, buffer_size);
		AnalogCH3_collected_data = GetAnalogChannelValue(aADC3ConvertedData, ADC_RANGE, buffer_size);
		ADC_ConvCplt = 0x0; /* reset flag "ADCs conversions complete" */
		return SUCCESS;
	}
	
}

/**
 * Функция преобразования в вольты
 * @param ACDxconvertedData			- указатель на массив, преобразованных данных
 */
static float32_t GetAnalogChannelValue(volatile uint16_t *ACDxconvertedData, uint32_t ADC_Range, uint32_t buffer_size)
{
	float32_t unfiltered_volts[buffer_size];
	float32_t filtered_volts[buffer_size];
	ADCdata_to_volts(ADC_Range, ACDxconvertedData, unfiltered_volts, buffer_size);
	DataFiltering(unfiltered_volts, filtered_volts, buffer_size);
	return AverageCalc(filtered_volts, buffer_size);
}

/**
 * Функция фильтрации входных данных 
 * @param pSrc указатель на входной массив
 * @param pDst	указатель на массив выходных данных
 */
static void DataFiltering(float32_t *pSrc, float32_t *pDst, uint32_t buffer_size)
{
	/****************** Останки от реализации фильтра IIR *************/
	/*настройка фильтра хорошо описана тут https://schaumont.dyn.wpi.edu/ece4703b21/lecture7.html#:~:text=considerable%20performance%20improvement.-,IIR%20Designs%20using%20ARM%20CMSIS%20DSP,-Just%20as%20with*/

	/*данные для IIR фильтра*/
	/*
	 #define IIR_FILTER_NUM_STAGES	2  // количество секций 2го порядка в фильтре
	 float64_t IIRfilterCoefficients[IIR_FILTER_NUM_STAGES * 5] = { 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579, 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579 };
	 float64_t IIRfilter_taps[4 * IIR_FILTER_NUM_STAGES];
	 //arm_biquad_casd_df1_inst_f32 IIRfilterInstance; //Структура для работы функции*/
	//arm_biquad_cascade_df1_init_f32(&IIRfilterInstance, IIR_FILTER_NUM_STAGES, IIRfilterCoefficients, IIRfilter_taps);
	/*arm_biquad_cascade_df1_f32(&IIRfilterInstance, aADCvoltsData, aIIRfilterConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE);
	 *********************************************************************/

	/*данные для FIR фильтра*/

	arm_fir_instance_f32 FIRfilterInstance; /*структура для работы функции фильтра*/
	
	float64_t FIRfilter_taps[FIR_FILTER_NUM_STAGES + buffer_size - 1];
	
	/*вызов инициализации фильтров*/
	arm_fir_init_f32(&FIRfilterInstance, FIR_FILTER_NUM_STAGES, FIRfilterCoefficients, FIRfilter_taps, buffer_size);
	
	/* закидывавем массив в фильтры*/

	arm_fir_f32(&FIRfilterInstance, pSrc, pDst, buffer_size);
}

/**
 * 
 * @param pSrc 			- pointer to source buffer
 * @param buffer_size	- source buffer size
 * @return average value from values in source buffer
 */
static float32_t AverageCalc(float32_t *pSrc, uint32_t buffer_size)
{
	float32_t average;
	float64_t sum, Source;
	average = 0.0; /* переменные для усреднения */
	sum = 0.0;
	
	for (uint32_t indx = 0; indx < buffer_size; indx++)
	{
		Source = (float64_t) pSrc[indx];
		sum += Source;
	}
	average = (float32_t) (sum / buffer_size);
	return average;
}

/**
 * 
 * @param ADC_max_value 	- 		максимальное число, которое может вернуть ADC
 * @param pSrc					-		pointer to source buffer
 * @param pDst					-		pointer to destination buffer
 * @param size_of_data_array	-	buffers size
 */
static void ADCdata_to_volts(uint32_t ADC_max_value, volatile uint16_t *pSrc, float32_t *pDst, uint32_t size_of_data_array)
{
	float64_t VoltADCCoeffitient, result;
	VoltADCCoeffitient = 3.3 / ADC_max_value; /* 3.3 - is the Vref in volts */
	for (uint32_t indx = 0; indx < size_of_data_array; indx++)
	{
		result = (pSrc[indx] * VoltADCCoeffitient);
		pDst[indx] = result;
	}
}

/*  колбэк прерывания по DMA, когда преобразована половина заданного массива */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer */
	if (hadc->Instance == ADC_1)
		SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC1ConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
	else if (hadc->Instance == ADC_2)
		SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC2ConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
	else if (hadc->Instance == ADC_3)
		SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC3ConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
	
}

/**
 * @brief  Conversion DMA full-transfer callback in non-blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer */
	if (hadc->Instance == ADC_1)
	{
		SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE / 2], ADC_CONVERTED_DATA_BUFFER_SIZE);
		ADC_ConvCplt |= 0x1; /* 0-bit set*/
	} else if (hadc->Instance == ADC_2)
	{
		SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC3ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE / 2], ADC_CONVERTED_DATA_BUFFER_SIZE);
		ADC_ConvCplt |= 0x2; /* 1-bit set*/
	} else if (hadc->Instance == ADC_3)
	{
		SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC3ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE / 2], ADC_CONVERTED_DATA_BUFFER_SIZE);
		ADC_ConvCplt |= 0x4; /* 2-bit set*/
	}
	HAL_TIM_Base_Stop(&TimForADC_Handle);
}

/* TODO запилить ERROR HANDLER*/
//void Error_Handler(void)
//{
//	while (1)
//	{
//	}
//}
