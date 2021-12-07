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

#endif /* ADC_DATA_COLLECTION_H_ */
