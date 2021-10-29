/*
 * MPU_cfg.c
 *
 *  Created on: 21 июн. 2021 г.
 *      Author: cableman
 */

void MPU__RegionConfig(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;
	/* Disable MPU */
	HAL_MPU_Disable();
	/* Configure RAM region as Region N°0, 8kB of size and R/W region */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x30000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Configure FLASH region as REGION N°1, 1MB of size and R/W region */
	MPU_InitStruct.BaseAddress = 0x08000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_1MB;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Configure FMC region as REGION N°2, 0.5GB of size, R/W region */
	MPU_InitStruct.BaseAddress = 0x60000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_512MB;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enable MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}
