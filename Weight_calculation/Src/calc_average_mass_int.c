#include "calc_average_mass_int.h"
#include  <stdlib.h>

void get_average_mass_int(weightFormulaIntInp_t *weightFormulaIntInp, uint32_t buffer_size)
{
	int_fast64_t LoadedSkip = 0;
	int_fast64_t Ni, Nu, Nw, F1, F3, preResult;
	
#if defined (TEST_ADC_LINEARITY)
	int_fast64_t testADC1average, testADC2average, testADC3average;
	testADC1average = 0;
	testADC2average = 0;
	testADC3average = 0;
#endif
	
	F1 = (int_fast64_t) (*weightFormulaIntInp->coefficientF1 * weightFormulaIntInp->intMultiplier);
	F3 = (int_fast64_t) (*weightFormulaIntInp->coefficientF3 * weightFormulaIntInp->intMultiplier);
	for (uint32_t indx = 0; indx < buffer_size; indx++)
	{
		Nu = ((int_fast64_t) weightFormulaIntInp->motorVoltage[indx] - *weightFormulaIntInp->ZeroOffsetMotorVoltage) * weightFormulaIntInp->intMultiplier;
		Ni = ((int_fast64_t) weightFormulaIntInp->motorCurrent[indx] - *weightFormulaIntInp->ZeroOffsetMotorCurrent) * weightFormulaIntInp->intMultiplier;
		Nw = abs((int_fast64_t) weightFormulaIntInp->motorSpeed[indx] - *weightFormulaIntInp->ZeroOffsetMotorSpeed) * weightFormulaIntInp->intMultiplier;
		LoadedSkip += (Ni * (Nu - Ni * F1 / weightFormulaIntInp->intMultiplier) / Nw);
#if defined (TEST_ADC_LINEARITY)
		testADC1average += (int_fast64_t) weightFormulaIntInp->motorVoltage[indx];
		testADC2average += (int_fast64_t) weightFormulaIntInp->motorCurrent[indx];
		testADC3average += (int_fast64_t) weightFormulaIntInp->motorSpeed[indx];
#endif
	}
	preResult = ((LoadedSkip / buffer_size - F3) / weightFormulaIntInp->intMultiplier);
	weightFormulaIntInp->result = (float) preResult * *weightFormulaIntInp->coefficientF2;
#if defined (TEST_ADC_LINEARITY)
	testADC1average /= buffer_size;
	testADC2average /= buffer_size;
	testADC3average /= buffer_size;
#endif
}
