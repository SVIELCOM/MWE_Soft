/*
 * adc_data_collection.h
 *
 *  Created on: Dec 3, 2021
 *      Author: cableman
 */

#ifndef ADC_DATA_COLLECTION_H_
#define ADC_DATA_COLLECTION_H_

#include "ADCandTIMconfig.h"
#include "arm_math.h" /* would'n work without this*/

#define __FPU_PRESENT 	1

#define	FIR_FILTER_ENABLED

#define 	FIR_FILTER_NUM_STAGES	21   /* количество секций фильтра*/
#define 	ADC_CONVERSION_COMPLETED		0x7
#define 	SUCCESS						1U
#define 	UNSUCCESS					0U

/* export variables declaration */
uint8_t ADC_ConvCplt; /* 0,1,3 bit used for ADC1,ADC2,ADC3 conversion complete flag*/
float32_t AnalogCH1_collected_data; 		//* adc1 collected, filtered, averaged data from analog channel 1 signal*/
float32_t AnalogCH2_collected_data;     //* adc1 collected, filtered, averaged data from analog channel 2 signal*/
float32_t AnalogCH3_collected_data;     //* adc1 collected, filtered, averaged data from analog channel 3 signal*/

/* Functions declaration */
void StartADCdataCollection(void);
uint8_t GetAllFreshAnalogChannelsValues(uint32_t buffer_size);

#endif /* ADC_DATA_COLLECTION_H_ */
