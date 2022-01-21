/*
 * weight_calc.h
 *
 *  Created on: Dec 13, 2021
 *      Author: cableman
 *      для понимания: вернулся писать этот модуль после перерыва в месяц, с полным пониманием того факта, 
 *      что новые весы нахуй никому не нужны.
 *      
 */

#ifndef WEIGHT_CALC_H_
#define WEIGHT_CALC_H_

#include "adc_data_collection.h"

/* the structure is needed for easy mapping of input data to data for the mass calculation */
typedef struct {
	float *motorSpeed;
	float *motorCurrent;
	float *motorVoltage;
	float *coefficientF1;
	float *coefficientF2;
	float *coefficientF3;
	uint8_t *direction;
	float result;
} weightFormula_t;

void getSkipWeight(weightFormula_t *inputStruct);

#endif /* WEIGHT_CALC_H_ */
