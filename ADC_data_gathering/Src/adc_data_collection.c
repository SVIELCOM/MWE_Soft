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

/*static Functions*/
static void Error_Handler(void);

void garbage(void)
{
	/******************Замутил тут шляпу для проверки фильтрации тупым усреднением и фильтром IIR *************/
	/*настройка фильтра хорошо описана тут https://schaumont.dyn.wpi.edu/ece4703b21/lecture7.html#:~:text=considerable%20performance%20improvement.-,IIR%20Designs%20using%20ARM%20CMSIS%20DSP,-Just%20as%20with*/

	/*данные для IIR фильтра*/
	/*
	 #define IIR_FILTER_NUM_STAGES	2  // количество секций 2го порядка в фильтре
	 float64_t IIRfilterCoefficients[IIR_FILTER_NUM_STAGES * 5] = { 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579, 1.00383902, -1.87017398, 0.87516701, 1.87065422, -0.87852579 };
	 float64_t IIRfilter_taps[4 * IIR_FILTER_NUM_STAGES];
	 */
	/*данные для FIR фильтра*/

	arm_biquad_casd_df1_inst_f32 IIRfilterInstance; /*структура для работы функции*/
	arm_fir_instance_f32 FIRfilterInstance; /*структура для работы функции*/
	
	static float32_t VoltADCCoeffitient; /*TODO: потом переделать на вычисление из заданного количества бит дискретизации*/
	VoltADCCoeffitient = 3.3f / 65536;
	
	float32_t VoltsAverage, FIRVoltsAverage; /* IIRVoltsAverage - removed */
	VoltsAverage = 0.0f; /* переменные для усреднения */
	
	/*вызов инициализации фильтров*/
	arm_fir_init_f32(&FIRfilterInstance, FIR_FILTER_NUM_STAGES, FIRfilterCoefficients, FIRfilter_taps, ADC_CONVERTED_DATA_BUFFER_SIZE);
	//	arm_biquad_cascade_df1_init_f32(&IIRfilterInstance, IIR_FILTER_NUM_STAGES, IIRfilterCoefficients, IIRfilter_taps);
	
	/************************************** это из while(1)*/

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

/** TODO разобраться с этими колбэками */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes */
	//SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC1ConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
}

/**
 * @brief  Conversion DMA half-transfer callback in non-blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
	//SCB_InvalidateDCache_by_Addr((uint32_t*) &aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE / 2], ADC_CONVERTED_DATA_BUFFER_SIZE);
}

/* TODO запилить ERROR HANDLER*/
static void Error_Handler(void)
{
	while (1)
	{
	}
}
