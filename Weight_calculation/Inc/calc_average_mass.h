/*
 * calc_average_mass.h
 *
 *  Created on: Feb 10, 2022
 *      Author: cableman
 */

#ifndef CALC_AVERAGE_MASS_H_
#define CALC_AVERAGE_MASS_H_

#include "weight_calc.h"

float get_average_mass(volatile uint16_t *MotVoltageACDxconvertedData, volatile uint16_t *MotCurrentACDxconvertedData, volatile uint16_t *MotSpeedACDxconvertedData, uint32_t ADC_Range,
		uint32_t buffer_size, float *f1, float *f2, float *f3, int_fast64_t *ZeroOffsetMotorVoltage, int_fast64_t *ZeroOffsetMotorCurrent, int_fast64_t *ZeroOffsetMotorSpeed);

#endif /* CALC_AVERAGE_MASS_H_ */
