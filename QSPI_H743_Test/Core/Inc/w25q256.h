/*
 * w25q256.h
 *
 *  Created on: Jul 7, 2021
 *      Author: cableman
 */

#ifndef INC_W25Q256_H_
#define INC_W25Q256_H_

#include "stm32h7xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition for QSPI clock resources */
/*
 #define QSPI_CLK_ENABLE()          __HAL_RCC_QSPI_CLK_ENABLE()
 #define QSPI_CLK_DISABLE()         __HAL_RCC_QSPI_CLK_DISABLE()
 #define QSPI_CS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOG_CLK_ENABLE()
 #define QSPI_CLK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
 #define QSPI_D0_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
 #define QSPI_D1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
 #define QSPI_D2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
 #define QSPI_D3_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

 #define QSPI_FORCE_RESET()         __HAL_RCC_QSPI_FORCE_RESET()
 #define QSPI_RELEASE_RESET()       __HAL_RCC_QSPI_RELEASE_RESET()
 */

/*QSPI prescaler define*/
#define QSPI_CLK_PRESCALER				24 /* 10 MHz. AHB3 - 240 MHz*/

/* W25Q256VJ size */
#define QSPI_FLASHSIZE					24 /*calculated for 32MB size */

/* Definition for QSPI Pins */
#define QSPI_CS_PIN                GPIO_PIN_6
#define QSPI_CS_GPIO_PORT          GPIOB
#define QSPI_CLK_PIN               GPIO_PIN_10
#define QSPI_CLK_GPIO_PORT         GPIOF
#define QSPI_IO0_PIN               GPIO_PIN_8
#define QSPI_IO0_GPIO_PORT         GPIOF
#define QSPI_IO1_PIN               GPIO_PIN_9
#define QSPI_IO1_GPIO_PORT         GPIOF
#define QSPI_IO2_PIN               GPIO_PIN_7
#define QSPI_IO2_GPIO_PORT         GPIOF
#define QSPI_IO3_PIN               GPIO_PIN_6
#define QSPI_IO3_GPIO_PORT         GPIOF
/*
 * this for Dual flash
 #define QSPI_D4_PIN                GPIO_PIN_7
 #define QSPI_D4_GPIO_PORT          GPIOE
 #define QSPI_D5_PIN                GPIO_PIN_8
 #define QSPI_D5_GPIO_PORT          GPIOE
 #define QSPI_D6_PIN                GPIO_PIN_9
 #define QSPI_D6_GPIO_PORT          GPIOE
 #define QSPI_D7_PIN                GPIO_PIN_10
 #define QSPI_D7_GPIO_PORT          GPIOE
 */

/* MT25TL01GHBA8ESF Micron memory */
/* Size of the flash */
#define QSPI_FLASH_SIZE                      		24
#define QSPI_PAGE_SIZE                       		256

/* Reset Operations */
#define RESET_ENABLE_CMD                     		0x66
#define DUMMY_RESET_ENABLE_CMD							0
#define RESET_DEVICE_CMD                     		0x99
#define DUMMY_RESET_DEVICE_CMD							0

/* Identification Operations */
#define READ_ID_CMD                          		0xAB
#define DUMMY_READ_ID_CMD									24
#define MANUFACTURER_DEVICE_ID               		0x90
#define DUMMY_MANUFACTURER_DEVICE_ID					0 /* there must to send address = 0x000000*/

#define MANUFACTURER_DEVICE_ID_DUAL_IO       		0x92
#define DUMMY_MANUFACTURER_DEVICE_ID_DUAL_IO			0 /* there must to send address = 0x000000*/
#define ALTB_MANUFACTURER_DEVICE_ID_DUAL_IO			0xF0

#define MANUFACTURER_DEVICE_ID_QUAD_IO       		0x94
#define DUMMY_MANUFACTURER_DEVICE_ID_QUAD_IO			4 /* there must to send address = 0x000000*/
#define ALTB_MANUFACTURER_DEVICE_ID_QUAD_IO			0xF0

#define JEDEC_ID												0x9F
#define DUMMY_JEDEC_ID										0

#define READ_UNIQUE_ID										0x4B
#define DUMMY_READ_UNIQUE_ID								32
// #define MULTIPLE_IO_READ_ID_CMD              0x
// #define READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x

/* Read Operations */
#define READ_DATA_CMD                        		0x03
#define DUMMY_READ_DATA_CMD								0
#define READ_4_BYTE_ADDR_CMD                 		0x13
#define DUMMY_READ_4_BYTE_ADDR_CMD						0

#define FAST_READ_CMD                        		0x0B
#define DUMMY_FAST_READ_CMD								8
#define FAST_READ_4_BYTE_ADDR_CMD            		0x0C
#define DUMMY_FAST_READ_4_BYTE_ADDR_CMD				8
#define DUAL_OUT_FAST_READ_CMD               		0x3B
#define DUMMY_DUAL_OUT_FAST_READ_CMD					8
#define DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD   		0x3C
#define DUMMY_DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD 	8

#define DUAL_INOUT_FAST_READ_CMD             		0xBB
#define DUMMY_DUAL_INOUT_FAST_READ_CMD					0
#define ALTB_DUAL_INOUT_FAST_READ_CMD					0xF0
#define DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD 		0xBC
#define DUMMY_DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0
#define ALTB_DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD	0xF0

#define QUAD_OUT_FAST_READ_CMD               		0x6B
#define DUMMY_QUAD_OUT_FAST_READ_CMD					8
#define QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD   		0x6C
#define DUMMY_QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD 	8

#define QUAD_INOUT_FAST_READ_CMD             		0xEB
#define DUMMY_QUAD_INOUT_FAST_READ_CMD					4
#define ALTB_QUAD_INOUT_FAST_READ_CMD					0xF0
#define QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD 		0xEC
#define DUMMY_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD	4
#define ALTB_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD	0xF0

#define SET_BURST_WITH_WRAP_CMD							0x77
#define DUMMY_SET_BURST_WITH_WRAP_CMD					6

/* Write Operations */
#define WRITE_ENABLE_CMD                     		0x06
#define WRITE_DISABLE_CMD                    		0x04
#define VOL_STATUS_REG_WRITE_ENABLE_CMD				0x50

/* Register Operations */
#define READ_STATUS_REG1_CMD                 		0x05
#define DUMMY_READ_STATUS_REG1_CMD						0
#define READ_STATUS_REG2_CMD                 		0x35
#define DUMMY_READ_STATUS_REG2_CMD						0
#define READ_STATUS_REG3_CMD                 		0x15
#define DUMMY_READ_STATUS_REG3_CMD						0
#define WRITE_STATUS_REG1_CMD                		0x01
#define DUMMY_WRITE_STATUS_REG1_CMD						0
#define WRITE_STATUS_REG2_CMD                		0x31
#define DUMMY_WRITE_STATUS_REG2_CMD						0
#define WRITE_STATUS_REG3_CMD                		0x11
#define DUMMY_WRITE_STATUS_REG3_CMD						0

#define SFDP_REGISTER_CMD									0x5A
#define DUMMY_SFDP_REGISTER_CMD							8

#define READ_SECURITY_REG_CMD								0x48
#define DUMMY_READ_SECURITY_REG_CMD						8
#define PROG_SECURITY_REG_CMD								0x42
#define DUMMY_PROG_SECURITY_REG_CMD						0
#define ERASE_SECURITY_REG_CMD							0x44
#define DUMMY_ERASE_SECURITY_REG_CMD					0

#define READ_EXT_ADDR_REG_CMD                		0xC8
#define DUMMY_READ_EXT_ADDR_REG_CMD						0
#define WRITE_EXT_ADDR_REG_CMD               		0xC5
#define DUMMY_WRITE_EXT_ADDR_REG_CMD					0

/* Program Operations */
#define PAGE_PROG_CMD                        		0x02
#define DUMMY_PAGE_PROG_CMD								0
#define PAGE_PROG_4_BYTE_ADDR_CMD            		0x12
#define DUMMY_PAGE_PROG_4_BYTE_ADDR_CMD				0

#define QUAD_IN_FAST_PROG_CMD                		0x32
#define DUMMY_QUAD_IN_FAST_PROG_CMD						0
#define QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD    		0x34
#define DUMMY_QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD		0

/* Erase Operations */
#define SECTOR_ERASE_CMD                  			0x20
#define SECTOR_ERASE_4_BYTE_ADDR_CMD      			0x21

#define BLOCK32K_ERASE_CMD                  			0x52

#define BLOCK64K_ERASE_CMD                   		0xD8
#define BLOCK64K_ERASE_4_BYTE_ADDR_CMD       		0xDC

#define BULK_ERASE_CMD                       		0xC7

#define PROG_ERASE_RESUME_CMD                		0x7A
#define PROG_ERASE_SUSPEND_CMD               		0x75

/* One-Time Programmable Operations */
//#define READ_OTP_ARRAY_CMD                   0x
//#define PROG_OTP_ARRAY_CMD                   0x
/* 4-byte Address Mode Operations */
#define ENTER_4_BYTE_ADDR_MODE_CMD           		0xB7
#define EXIT_4_BYTE_ADDR_MODE_CMD            		0xE9

/* Power UP and power release commands*/
#define POWER_DOWN_CMD										0xB9
#define POWER_RELEASE_CMD									0xAB

/* Block Lock and Unlock commands */
#define GLOBAL_BLOCK_LOCK_CMD								0x7E
#define GLOBAL_BLOCK_UNLOCK_CMD							0x98
#define READ_BLOCK_LOCK_CMD								0x3D
#define DUMMY_READ_BLOCK_LOCK_CMD						0

#define INDIVIDUAL_BLOCK_LOCK_CMD						0x36
#define INDIVIDUAL_BLOCK_UNLOCK_CMD						0x39

/* End address of the QSPI memory */
#define QSPI_END_ADDR              (1 << QSPI_FLASH_SIZE)

/* Size of buffers */
#define BUFFERSIZE                 (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)        (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Functions prototypes */
void W25Q256_QUADSPI_Init(QSPI_HandleTypeDef *hqspi);
void W25Q256_Enter_4ByteAddrMode(QSPI_HandleTypeDef *hqspi);
uint16_t W25Q256_GetMftrDeviceID_QuadIO(QSPI_HandleTypeDef *hqspi);

#endif /* INC_W25Q256_H_ */

