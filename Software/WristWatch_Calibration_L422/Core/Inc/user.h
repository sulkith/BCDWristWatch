/*
 * user.h
 *
 *  Created on: 16.10.2020
 *      Author: sulkith
 */

#ifndef SRC_USER_H_
#define SRC_USER_H_
#include "main.h"
#define secondsToMeasure 300

#define WatchVersCodeAddress (FLASH_BASE + FLASH_SIZE - FLASH_PAGE_SIZE + 0x10)
void writeFlash(uint32_t Address, uint64_t data);
void initIOs();
uint64_t readFlash(uint32_t Address);
uint32_t DWT_Delay_Init(void);

void showError();
void showFinish();
void testLEDs();
void init_IOs();

void showRemainingTime(uint16_t TimeToGo);

__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds) {
	uint32_t clk_cycle_start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - clk_cycle_start) < microseconds)
		;
}



#endif /* SRC_USER_H_ */
