/*
 * w25q256.c
 *
 *  Created on: 7 июл. 2021 г.
 *      Author: cableman
 */

#include "w25q256.h"
#include "main.h"

extern QSPI_HandleTypeDef hqspi;

// uint32_t address = 0;

// volatile uint8_t step = 0;
// volatile uint8_t CmdCplt, RxCplt, TxCplt, StatusMatch, TimeOut;

/* Buffer used for transmission */
uint8_t aTxBuffer[] =
		" ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT****  ****QSPI communication based on IT**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[256];

/* Private function prototypes -----------------------------------------------*/
// static void Error_Handler(void);
static void W25Q256_WriteEnable(QSPI_HandleTypeDef *hqspi);
static void W25Q256_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief QUADSPI Initialization Function
 * @param None
 * @retval None
 */
void W25Q256_QUADSPI_Init(QSPI_HandleTypeDef *hqspi)
{

	/* TODO: переделать функцию без использования HAL.
	 * Переделать так, чтобы функция поддягивала данные о частоте тактирования из железа
	 */

	/* QUADSPI parameter configuration*/
	hqspi->Instance = QUADSPI;
	hqspi->Init.ClockPrescaler = QSPI_CLK_PRESCALER;
	hqspi->Init.FifoThreshold = 4;
	hqspi->Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
	hqspi->Init.FlashSize = QSPI_FLASHSIZE;
	hqspi->Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	hqspi->Init.ClockMode = QSPI_CLOCK_MODE_0;
	hqspi->Init.FlashID = QSPI_FLASH_ID_1;
	hqspi->Init.DualFlash = QSPI_DUALFLASH_DISABLE;
	if (HAL_QSPI_Init(hqspi) != HAL_OK)
	{
		Error_Handler();
	}
}

static void W25Q256_WriteEnable(QSPI_HandleTypeDef *QSPI_Handler)
{
	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Enable write operations ------------------------------------------ */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = WRITE_ENABLE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(QSPI_Handler, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	/* Configure automatic polling mode to wait for write enabling ---- */
	sConfig.Match = 0x02;
	sConfig.Mask = 0x02;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.Instruction = READ_STATUS_REG1_CMD;
	sCommand.DataMode = QSPI_DATA_1_LINE;

	if (HAL_QSPI_AutoPolling(QSPI_Handler, &sCommand, &sConfig, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
 * The Enter 4-Byte Address Mode function will
 * allow 32-bit address (A31-A0) to be used to
 * access the memory array beyond 128Mb.
 * */
void W25Q256_Enter_4ByteAddrMode(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef sCommand;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = ENTER_4_BYTE_ADDR_MODE_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
}

uint16_t W25Q256_GetMftrDeviceID_QuadIO(QSPI_HandleTypeDef *hqspi)
{
	uint16_t MftrDeviceID = 0;
	QSPI_CommandTypeDef sCommand;
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = MANUFACTURER_DEVICE_ID_QUAD_IO;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.AddressSize = QSPI_ADDRESS_24_BITS; /*TODO: в дальнейшем надо проверять, в каком режиме адресации находится FLASH*/
	sCommand.Address = 0x000000U; /*и передавать столько бит адреса, сколько установлено в регистре ADP */
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
	sCommand.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytes = ALTB_MANUFACTURER_DEVICE_ID_QUAD_IO;
	sCommand.DataMode = QSPI_DATA_4_LINES;
	sCommand.NbData = 0x2;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.DummyCycles = DUMMY_MANUFACTURER_DEVICE_ID_QUAD_IO;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_QSPI_Receive(hqspi, (uint8_t*) &MftrDeviceID, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
	return MftrDeviceID;
}

/**
 * @brief  This function read the SR of the memory and wait the EOP.
 * @param  hqspi: QSPI handle
 * @retval None
 */
static void W25Q256_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Configure automatic polling mode to wait for memory ready ------ */
	sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction = READ_STATUS_REG1_CMD;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	sConfig.Match = 0x00;
	sConfig.Mask = 0x01;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling_IT(hqspi, &sCommand, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

