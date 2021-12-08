/*
 * adc_data_collection.h
 *
 *  Created on: Dec 3, 2021
 *      Author: cableman
 */

#ifndef ADC_DATA_COLLECTION_H_
#define ADC_DATA_COLLECTION_H_

#include "arm_math.h"
#include "ADCandTIMconfig.h"

#define __FPU_PRESENT 	1

#define PCB_VERSION_0_1						//для первой версии платы 

// #define ADC_OVERSAMPLING

/* TODO: задействовать использование этих макросов, чтобы иметь возможность переключать разрешение АЦП */
#define RANGE_8BITS                    ((uint32_t)  255) 	/* Max digital value with a full range of 8 bits */
#define RANGE_10BITS                   ((uint32_t) 1023)    /* Max digital value with a full range of 12 bits */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max digital value with a full range of 12 bits */
#define RANGE_14BITS                   ((uint32_t)16383)    /* Max digital value with a full range of 14 bits */
#define RANGE_16BITS                   ((uint32_t)65535)    /* Max digital value with a full range of 16 bits */

#define ADC_CONVERTED_DATA_BUFFER_SIZE ((uint32_t)  32)		/* Size of array aADCxConvertedData[] */

#define FIR_FILTER_NUM_STAGES	21  /* количество секций фильтра*/

/*коэффициенты фильтра*/
float32_t FIRfilterCoefficients[FIR_FILTER_NUM_STAGES] = { 0.0072524808347225189208984375, 0.009322776459157466888427734375, 0.01530767977237701416015625, 0.02464949898421764373779296875,
		0.0364511311054229736328125, 0.04956446588039398193359375, 0.062704540789127349853515625, 0.07457792758941650390625, 0.084012426435947418212890625, 0.0900747776031494140625,
		0.09216459095478057861328125, 0.0900747776031494140625, 0.084012426435947418212890625, 0.07457792758941650390625, 0.062704540789127349853515625, 0.04956446588039398193359375,
		0.0364511311054229736328125, 0.02464949898421764373779296875, 0.01530767977237701416015625, 0.009322776459157466888427734375, 0.0072524808347225189208984375 };

float32_t FIRfilter_taps[FIR_FILTER_NUM_STAGES + ADC_CONVERTED_DATA_BUFFER_SIZE - 1];

#endif /* ADC_DATA_COLLECTION_H_ */
