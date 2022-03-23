/*
 * adc_data_collection.h
 *
 *  Created on: Dec 3, 2021
 *      Author: cableman
 */

#ifndef ADC_DATA_COLLECTION_H_
#define ADC_DATA_COLLECTION_H_

#include "ADCandTIMconfig.h"
#include "arm_math.h" 												/* would'n work without this*/

#define __FPU_PRESENT 						1						/* must be there for fpu-based instructions  */

//#define	FIR_FILTER_ENABLED  									/* enable or disable FIR filter */

#define 	FIR_FILTER_NUM_STAGES			21   					/* количество секций фильтра*/
#define 	ADC_CONVERSION_COMPLETED		0x7
#define 	SUCCESS								1U		
#define 	UNSUCCESS							0U
#define  VOLTAGE_OFFSET						1.65d					//общее смещение относительно 0. (опорное напряжение ОУ) в продакшене испол

/* export variables declaration */
uint8_t ADC_ConvCplt; 							//* 0,1,3 bit used for ADC1,ADC2,ADC3 conversion complete flag*/
float32_t AnalogCH1_collected_data; 		//* adc1 collected, filtered, averaged data from analog channel 1 signal*/
float32_t AnalogCH2_collected_data;       //* adc1 collected, filtered, averaged data from analog channel 2 signal*/
float32_t AnalogCH3_collected_data;       //* adc1 collected, filtered, averaged data from analog channel 3 signal*/

/* Functions declaration */
void StartADCdataCollection(void);
uint8_t GetAllFreshAnalogChannelsValues(uint32_t buffer_size);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* ADC_DATA_COLLECTION_H_ */
