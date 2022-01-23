/*
 * utility.c
 *
 *  Created on: 16.10.2020
 *      Author: sulkith
 */
#include "main.h"

void writeFlash(uint32_t Address, uint64_t data) {
//	uint32_t Address = FLASH_BASE+FLASH_SIZE-FLASH_PAGE_SIZE;
	HAL_StatusTypeDef status = 0;
	uint64_t currdata = *((uint64_t*) Address);
	if (data != currdata) {
		status = HAL_FLASH_Unlock();
		FLASH_EraseInitTypeDef erase = { 0 };
		erase.TypeErase = FLASH_TYPEERASE_PAGES;
		erase.Banks = FLASH_BANK_1;
		erase.Page = (Address - FLASH_BASE) / PAGESIZE;
		erase.NbPages = 1;
		uint32_t Error = 0;
		if(currdata != 0xFFFFFFFFFFFFFFFF)
		{
			status = HAL_FLASHEx_Erase(&erase, &Error);
		}
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, data);
		status = HAL_FLASH_Lock();
		UNUSED(status);
	}
}
uint64_t readFlash(uint32_t Address) {
	return *((uint64_t*) Address);
}

uint32_t DWT_Delay_Init(void) {
	/* Disable TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
	/* Enable TRC */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
	/* Disable clock cycle counter */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
	/* Enable  clock cycle counter */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
	/* Reset the clock cycle counter value */
	DWT->CYCCNT = 0;
	/* 3 NO OPERATION instructions */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	volatile uint8_t a;
	a++;
	a++;
	a++;
	/* Check if clock cycle counter has started */
	if (DWT->CYCCNT) {
		return 0; /*clock cycle counter started*/
	} else {
		return 1; /*clock cycle counter not started*/
	}
}
