/*
 * max7219.h
 *
 *  Created on: May 11, 2019
 *      Author: tabur
 */

#ifndef MAX7219_H_
#define MAX7219_H_

#include "main.h"
#include "stdbool.h"

#define NUMBER_OF_DIGITS_CHIP	8
#define FIRST_CHIP	1
#define SECOND_CHIP	2
#define SPI_PORT			hspi1

#define DMA_BUFFER __attribute__((section(".dma_buffer")))

extern SPI_HandleTypeDef SPI_PORT;

typedef enum {
	REG_NO_OP = 0x00,
	REG_DIGIT_0 = 0x01,
	REG_DIGIT_1 = 0x02,
	REG_DIGIT_2 = 0x03,
	REG_DIGIT_3 = 0x04,
	REG_DIGIT_4 = 0x05,
	REG_DIGIT_5 = 0x06,
	REG_DIGIT_6 = 0x07,
	REG_DIGIT_7 = 0x08,
	REG_DECODE_MODE = 0x09,
	REG_INTENSITY = 0x0A,
	REG_SCAN_LIMIT = 0x0B,
	REG_SHUTDOWN = 0x0C,
	REG_DISPLAY_TEST = 0x0F,
} MAX7219_REGISTERS;

void max7219_Init(uint8_t intensivity);
void max7219_SetIntensivity(uint8_t intensivity);
void max7219_Clean(void);
void max7219_SendData(uint8_t addr, uint8_t data);
void max7219_Turn_On(void);
void max7219_Turn_Off(void);
void max7219_Decode_On(void);
void max7219_Decode_Off(void);
void max7219_PrintCharToBuf(uint8_t position, char character, bool point);
uint8_t max7219_PrintItosToBuf(uint8_t position, int value);
uint8_t max7219_PrintNtosToBuf(uint8_t position, uint32_t value, uint8_t n);
uint8_t max7219_PrintFtosToBuf(uint8_t position, float value, uint8_t n);
void max7219_FillDispBuf(uint8_t value);
void max7219_SendToDispBuf(uint8_t position, uint8_t value);
void max7219_SendDispBuf();

#endif /* MAX7219_H_ */
