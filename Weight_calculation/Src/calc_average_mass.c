/*
 * calc_average_mass.c
 *
 *  Created on: Feb 10, 2022
 *      Author: cableman
 */

#include "calc_average_mass.h"
static double getADCvoltage(uint16_t ADCconvertedData, double VoltADCCoeffitient);

float get_average_mass(volatile uint16_t *MotVoltageACDxconvertedData, volatile uint16_t *MotCurrentACDxconvertedData, volatile uint16_t *MotSpeedACDxconvertedData, uint32_t ADC_Range,
		uint32_t buffer_size, float *f1, float *f2, float *f3)
{
	double VoltADCCoeffitient, MotorVoltage, MotorCurrent, MotorSpeed, mass;
	weightFormula_t weight_handler;
	VoltADCCoeffitient = 3.3 / ADC_Range;
	mass = 0.0d;
	for (uint32_t indx = 0; indx < buffer_size; indx++)
	{
		MotorVoltage = getADCvoltage(MotVoltageACDxconvertedData[indx], VoltADCCoeffitient); /* 3.3 - is the Vref in volts */
		MotorCurrent = getADCvoltage(MotCurrentACDxconvertedData[indx], VoltADCCoeffitient);
		MotorSpeed = getADCvoltage(MotSpeedACDxconvertedData[indx], VoltADCCoeffitient);
		weight_handler.coefficientF1 = f1;
		weight_handler.coefficientF2 = f2;
		weight_handler.coefficientF3 = f3;
		weight_handler.motorCurrent = &MotorCurrent;
		weight_handler.motorVoltage = &MotorVoltage;
		weight_handler.motorSpeed = &MotorSpeed;
		getSkipWeight(&weight_handler);
		mass += weight_handler.result;
	}
	return (float) mass / buffer_size;
}

static double getADCvoltage(uint16_t ADCconvertedData, double VoltADCCoeffitient)
{
	return (ADCconvertedData * VoltADCCoeffitient - VOLTAGE_OFFSET);
}
