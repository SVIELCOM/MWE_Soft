/*
 * max7219.c
 *
 *  Created on: May 11, 2019
 *      Author: tabur & SVIELCOM
 */

#include "max7219.h"
/*
 * буфер дисплея в памяти RAM_D2 (), т.к. DMA2 не работает с RAM
 * в скрипте для линкера необходимо добавить необходимые строки для этого
 *
 */
static DMA_BUFFER uint8_t DispBuffer[32] = { 0 }; //буфер дисплея в памяти RAM_D2 ()

static uint8_t getCodeFromChar(char symb);
static void max7219_sendCommandToDispBuf(uint8_t reg_no, uint8_t command);
static uint32_t lcdPow10(uint8_t n);

void max7219_Init(uint8_t intensivity)
{
	max7219_Turn_On();
	max7219_sendCommandToDispBuf(REG_SCAN_LIMIT, NUMBER_OF_DIGITS_CHIP - 1);
	max7219_SendDispBuf();
	max7219_SetIntensivity(intensivity);
	max7219_Decode_Off();
	max7219_Clean();
}

void max7219_SetIntensivity(uint8_t intensivity)
{
	if (intensivity > 0x0F)
	{
		return;
	}
	max7219_sendCommandToDispBuf(REG_INTENSITY, intensivity);
	max7219_SendDispBuf();
}

void max7219_Clean()
{
	uint8_t clear = 0x00;

	max7219_FillDispBuf(clear);
	max7219_SendDispBuf();
}

void max7219_SendData(uint8_t addr, uint8_t data)
{
	uint16_t send_data = 0;
	send_data |= (addr << 8) | data;

	HAL_SPI_Transmit(&SPI_PORT, (uint8_t*) &send_data, 1, HAL_MAX_DELAY);
}

void max7219_Turn_On(void)
{
	max7219_sendCommandToDispBuf(REG_SHUTDOWN, 0x01);
	max7219_SendDispBuf();
}

void max7219_Turn_Off(void)
{
	max7219_sendCommandToDispBuf(REG_SHUTDOWN, 0x00);
	max7219_SendDispBuf();
}

void max7219_Decode_Off(void)
{
	max7219_sendCommandToDispBuf(REG_DECODE_MODE, 0x0);
	max7219_SendDispBuf();
}

void max7219_PrintCharToBuf(uint8_t position, char character, bool point)
{
	if (position > (NUMBER_OF_DIGITS_CHIP * 2))
	{
		return;
	}
	if (point)
	{
		max7219_SendToDispBuf(position, getCodeFromChar(character) | (1 << 7));
	} else
	{
		max7219_SendToDispBuf(position, getCodeFromChar(character) & (~(1 << 7)));
	}
}

uint8_t max7219_PrintItosToBuf(uint8_t position, int value)
{
	int32_t i;

	if (value < 0)
	{
		if (position > 0)
		{
			max7219_PrintCharToBuf(position, '-', false);
			position--;
		}
		value = -value;
	}

	i = 1;

	while ((value / i) > 9)
	{
		i *= 10;
	}

	if (position > 0)
	{
		max7219_PrintCharToBuf(position, (char) (value / i + 48), false);
		position--;
	}

	i /= 10;

	while (i > 0)
	{
		if (position > 0)
		{
			max7219_PrintCharToBuf(position, (char) ((value % (i * 10)) / i + 48), false);
			position--;
		}

		i /= 10;
	}
	return position;
}

uint8_t max7219_PrintNtosToBuf(uint8_t position, uint32_t value, uint8_t n)
{
	if (n > 0u)
	{
		uint32_t i = lcdPow10(n - 1u);

		while (i > 0u) /* Display at least one symbol */
		{
			if (position > 0u)
			{
				max7219_PrintCharToBuf(position, (char) ((value / i) % 10u + 48), false);
				position--;
			}

			i /= 10u;
		}
	}

	return position;
}

/*
 * вывод числа с точкой
 */
uint8_t max7219_PrintFtosToBuf(uint8_t position, float value, uint8_t n)
{
	if (n > 4)
	{
		n = 4;
	}

	if (value < 0.0)
	{
		if (position > 0)
		{
			max7219_PrintCharToBuf(position, '-', false);
			//max7219_SendToDispBuf(position, MINUS);
			position--;
		}
		value = -value;
	}

	position = max7219_PrintItosToBuf(position, (int32_t) value);

	if (n > 0u)
	{
		max7219_PrintCharToBuf(position + 1, (char) (((int32_t) value) % 10 + 48), true);

		position = max7219_PrintNtosToBuf(position, (uint32_t) (value * (float) lcdPow10(n)), n);
	}

	return position;
}

/*возведение в степень*/
static uint32_t lcdPow10(uint8_t n)
{
	uint32_t retval = 1u;

	while (n > 0u)
	{
		retval *= 10u;
		n--;
	}

	return retval;
}

static void max7219_sendCommandToDispBuf(uint8_t reg_no, uint8_t command)
{
	for (uint8_t i = 0; i < 4; i = i + 2)
	{
		DispBuffer[i] = command;
		DispBuffer[i + 1] = reg_no;
	}
	for (uint8_t i = 4; i < 32; i = i + 4) //буфер 32 байта по 16 байт на чип
	{
		for (uint8_t n = 0; n < 4; n = n + 2)
		{
			DispBuffer[i + n] = 0; 	//остальное заливаем 0-ми
			DispBuffer[i + n + 1] = 0;
		}
	}
}

static uint8_t getCodeFromChar(char symb)
{
	uint8_t code;
	switch (symb)
		{
		case ' ':
			code = 0x00;
			break;
		case '0':
			code = 0x7e;
			break;
		case '1':
			code = 0x30;
			break;
		case '2':
			code = 0x6d;
			break;
		case '3':
			code = 0x79;
			break;
		case '4':
			code = 0x33;
			break;
		case '5':
			code = 0x5b;
			break;
		case '6':
			code = 0x5f;
			break;
		case '7':
			code = 0x70;
			break;
		case '8':
			code = 0x7f;
			break;
		case '9':
			code = 0x7b;
			break;
		case '-':
			code = 0x01;
			break;
		case '_':
			code = 0x08;
			break;
		case 'A':
			code = 0x77;
			break;
		case 'B':
			code = 0x1f;
			break;
		case 'C':
			code = 0x77;
			break;
		case 'D':
			code = 0x3d;
			break;
		case 'E':
			code = 0x4f;
			break;
		case 'F':
			code = 0x47;
			break;
		case 'G':
			code = 0x7b;
			break;
		case 'H':
			code = 0x37;
			break;
		case 'I':
			code = 0x06;
			break;
		case 'J':
			code = 0x38;
			break;
		case 'K':
			code = 0x07;
			break;
		case 'L':
			code = 0x0e;
			break;
		case 'M':
			code = 0x40; //no symbol!
			break;
		case 'N':
			code = 0x15;
			break;
		case 'O':
			code = 0x3d;
			break;
		case 'P':
			code = 0x67;
			break;
		case 'Q':
			code = 0x73;
			break;
		case 'R':
			code = 0x40; //no symbol!
			break;
		case 'S':
			code = 0x5b;
			break;
		case 'T':
			code = 0x0f;
			break;
		case 'U':
			code = 0x1c;
			break;
		case 'V':
			code = 0x3e;
			break;
		case 'W':
			code = 0x3f;
			break;
		case 'X':
			code = 0x27;
			break;
		case 'Y':
			code = 0x13;
			break;
		case 'Z':
			code = 0x4d;
			break;
		default:
			code = 0x49; //not existed symbol
		}

	return code;
}

/*
 * функция отформатирует буфер и заполнит все разряды буфера дисплея одним символом value
 *
 */
void max7219_FillDispBuf(uint8_t value)
{
	for (uint8_t i = 0; i < 32; i = i + 4) //буфер 32 байта по 16 байт на чип
	{
		for (uint8_t n = 0; n < 4; n = n + 2)
		{
			DispBuffer[i + n] = value; 	//буфер надо формировать так, чтобы первым шло значение регистра
			DispBuffer[i + n + 1] = i / 4 + 1;	//номер регистра (разряда) для первого чипа
		}
	}
}

void max7219_SendToDispBuf(uint8_t position, uint8_t value)
{
	if (position > (NUMBER_OF_DIGITS_CHIP * 2))
		return;
	if (position > 8)
	{
		DispBuffer[((position - 8) * 4 - 2)] = value;
	} else
	{
		DispBuffer[(position * 4 - 4)] = value;
	}
}

/*
 * send buffer to chips
 */
void max7219_SendDispBuf()
{

	HAL_SPI_Transmit_DMA(&SPI_PORT, DispBuffer, 8);
	HAL_Delay(1); /* TODO: УБРАТЬ после допиливания правильной реализации DMA*/
// HAL_SPI_Transmit(&SPI_PORT, DispBuffer, 8, HAL_MAX_DELAY); // send using polling mode
}
