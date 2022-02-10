/*
 * weight_calc.c
 *
 *  Created on: Dec 13, 2021
 *      Author: cableman
 */

#include "weight_calc.h"
#include <math.h>

/**
 * 
 * @param inputStruct
 */
void getSkipWeight(weightFormula_t *inputStruct)
{
	double result, NI, NU, Nw, F1, F2, F3;     //* пока для понимания вкорячивания формулы создал переменные эти */
	NI = (double) *inputStruct->motorCurrent;
	NU = (double) *inputStruct->motorVoltage;
	Nw = fabs((double) *inputStruct->motorSpeed);
	F1 = (double) *inputStruct->coefficientF1;
	F2 = (double) *inputStruct->coefficientF2;
	F3 = (double) *inputStruct->coefficientF3;
	
	result = ((NI * (NU - NI * F1) / Nw) - F3) * F2;
	inputStruct->result = (float) result;
}
