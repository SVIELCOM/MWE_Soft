/*
 * calc_average_mass_int.h
 *
 *  Created on: 16 февр. 2022 г.
 *      Author: cableman
 */

#ifndef CALC_AVERAGE_MASS_INT_H_
#define CALC_AVERAGE_MASS_INT_H_

#include "adc_data_collection.h"
#define TEST_ADC_LINEARITY  //задефайнить, чтобы иметь среднее значение выходов АЦП

/* the structure is needed for easy mapping of input data to data for the mass calculation */
typedef struct {
	volatile uint16_t *motorSpeed;
	volatile uint16_t *motorCurrent;
	volatile uint16_t *motorVoltage;
	int_fast64_t *ZeroOffsetMotorVoltage;
	int_fast64_t *ZeroOffsetMotorCurrent;
	int_fast64_t *ZeroOffsetMotorSpeed;
	int_fast64_t intMultiplier;
	float *coefficientF1;
	float *coefficientF2;
	float *coefficientF3;
	float result;
} weightFormulaIntInp_t;

void get_average_mass_int(weightFormulaIntInp_t *weightFormulaIntInp, uint32_t buffer_size);

#endif /* CALC_AVERAGE_MASS_INT_H_ */
