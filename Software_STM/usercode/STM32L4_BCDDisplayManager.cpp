/*
 * STM32L4_BCDDisplayManager.cpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */

#include "STM32L4_BCDDisplayManager.hpp"
#include "SleepM.hpp"
#include "main.h"
#include "pins.hpp"

const uint16_t DISP_0 = 0;
const uint16_t DISP_1 = (HS1_Pin);
const uint16_t DISP_2 = (HS2_Pin);
const uint16_t DISP_3 = (HS1_Pin | HS2_Pin);
const uint16_t DISP_4 = (HS4_Pin);
const uint16_t DISP_5 = (HS4_Pin | HS1_Pin);
const uint16_t DISP_6 = (HS4_Pin | HS2_Pin);
const uint16_t DISP_7 = (HS4_Pin | HS2_Pin | HS1_Pin);
const uint16_t DISP_8 = (HS8_Pin);
const uint16_t DISP_9 = (HS8_Pin | HS1_Pin);
const uint16_t DISP_A = (HS8_Pin | HS2_Pin);
const uint16_t DISP_B = (HS8_Pin | HS2_Pin | HS1_Pin);
const uint16_t DISP_C = (HS8_Pin | HS4_Pin);
const uint16_t DISP_D = (HS8_Pin | HS4_Pin | HS1_Pin);
const uint16_t DISP_E = (HS8_Pin | HS4_Pin | HS2_Pin);
const uint16_t DISP_F = (HS8_Pin | HS4_Pin | HS2_Pin | HS1_Pin);
const uint8_t LED_Brightness = 240;


const uint16_t numToPort[] = { DISP_0, // 0
		DISP_1, // 1
		DISP_2, // 2
		DISP_3, // 3
		DISP_4, // 4
		DISP_5, // 5
		DISP_6, // 6
		DISP_7, // 7
		DISP_8, // 8
		DISP_9, // 9
		DISP_A, // A
		DISP_B, // B
		DISP_C, // C
		DISP_D, // D
		DISP_E, // E
		DISP_F // F
		};
//H10 H01
const uint32_t mask_for_pins = (((HS8_Pin | HS4_Pin | HS2_Pin | HS1_Pin)) << 16);
const uint32_t mask_clear = (((HS8_Pin | HS4_Pin | HS2_Pin | HS1_Pin)
		| (LSH10_Pin) | (LSH01_Pin) | (LSM10_Pin) | (LSM01_Pin)) << 16);
const uint32_t Column[] = { mask_for_pins | (LSH10_Pin << 16) | (LSH01_Pin)
		| (LSM10_Pin) | (LSM01_Pin), // H10 Column
mask_for_pins | (LSH01_Pin << 16) | (LSH10_Pin) | (LSM10_Pin) | (LSM01_Pin), // H01 Column
mask_for_pins | (LSM10_Pin << 16) | (LSH10_Pin) | (LSH01_Pin) | (LSM01_Pin), // M10 Column
mask_for_pins | (LSM01_Pin << 16) | (LSH10_Pin) | (LSH01_Pin) | (LSM10_Pin) // M01 Column
};
void STM32L4_BCDDisplayManager::executeSleepSubscription() {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			LSH01_Pin | LSH10_Pin | HS1_Pin | HS2_Pin | HS8_Pin | HS4_Pin
					| LSM01_Pin | LSM10_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BMA_CS_GPIO_Port, BMA_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pins : PA1 PA7 PA8 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : LSH01_Pin LSH10_Pin HS1_Pin HS2_Pin
	 HS8_Pin HS4_Pin LSM01_Pin LSM10_Pin */
	GPIO_InitStruct.Pin = LSH01_Pin | LSH10_Pin | HS1_Pin | HS2_Pin | HS8_Pin
			| HS4_Pin | LSM01_Pin | LSM10_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB7 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : BMA_CS_Pin */
	GPIO_InitStruct.Pin = BMA_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BMA_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PH3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	while (LED_Brightness >= 250)
		showERROR(0xC, 1); //EC01
}
void STM32L4_BCDDisplayManager::init() {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			LSH01_Pin | LSH10_Pin | HS1_Pin | HS2_Pin | HS8_Pin | HS4_Pin
					| LSM01_Pin | LSM10_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BMA_CS_GPIO_Port, BMA_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pins : PA1 PA7 PA8 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : LSH01_Pin LSH10_Pin HS1_Pin HS2_Pin
	 HS8_Pin HS4_Pin LSM01_Pin LSM10_Pin */
	GPIO_InitStruct.Pin = LSH01_Pin | LSH10_Pin | HS1_Pin | HS2_Pin | HS8_Pin
			| HS4_Pin | LSM01_Pin | LSM10_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB7 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : BMA_CS_Pin */
	GPIO_InitStruct.Pin = BMA_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BMA_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PH3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	while (LED_Brightness >= 250)
		showERROR(0xC, 1); //EC01

	//SleepM::getInstance()->subscribe(this);
}
/**
 * https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
 * @brief  This function provides a delay (in microseconds)
 * @param  microseconds: delay in microseconds
 */
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds) {
	uint32_t clk_cycle_start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - clk_cycle_start) < microseconds)
		;
}

void showLEDs(uint16_t DisplayBuffer[], uint16_t duration, uint8_t perc) {
	for (uint16_t j = 0; j < duration; ++j)
		for (uint8_t i = 0; i < 4; ++i) {
			//PORTD=DisplayBuffer[i]|((0b00111100)&(~(1<<(2+i))));
			GPIOA->BSRR = DisplayBuffer[3 - i] | Column[i];
			DWT_Delay_us(perc);
			GPIOA->BSRR = mask_clear;
			DWT_Delay_us(255 - perc);
		}
}
void STM32L4_BCDDisplayManager::show() {
	uint8_t BlinkOn = (HAL_GetTick() / 150) % 2;
	uint16_t DisplayBuffer[4];
	switch (request.getType()) {
	case Empty:
		return; // no need to stay awake for long
	case FadeIn:
	case Time:
	case SetHour:
	case SetMinute:
	case Debouncing:
		DisplayBuffer[3] = numToPort[request[0] / 10];
		DisplayBuffer[2] = numToPort[request[0] % 10];
		DisplayBuffer[1] = numToPort[request[1] / 10];
		DisplayBuffer[0] = numToPort[request[1] % 10];
		if (request[0] == 0 && request[1] == 0) {
			DisplayBuffer[3] = numToPort[2];
			DisplayBuffer[2] = numToPort[4];
			DisplayBuffer[1] = numToPort[0];
			DisplayBuffer[0] = numToPort[0];
		}
			if(BlinkOn == 0 &&request.getType() == SetHour)DisplayBuffer[3]|=DISP_8;
			if(BlinkOn == 0 &&request.getType() == SetMinute)DisplayBuffer[1]|=DISP_8;
			if(BlinkOn == 0 &&request.getType() == Debouncing)
			{
				DisplayBuffer[3]|=DISP_8;
				DisplayBuffer[1]|=DISP_8;
			}
		break;
	case Time_s:
		DisplayBuffer[3] = numToPort[request[1] / 10];//Minutes and Seconds
		DisplayBuffer[2] = numToPort[request[1] % 10];
		DisplayBuffer[1] = numToPort[request[2] / 10];
		DisplayBuffer[0] = numToPort[request[2] % 10];
		break;
	case Date:
	case SetDay:
	case SetMonth:
	case DebouncingDate:
		DisplayBuffer[3] = DISP_3;
		DisplayBuffer[2] = numToPort[request[0] / 10];
		DisplayBuffer[1] = numToPort[request[0] % 10];
		DisplayBuffer[0] = numToPort[request[1] % 16];
		if(BlinkOn == 0 &&request.getType() == SetDay)DisplayBuffer[2]|=DISP_8;
		if(BlinkOn == 0 &&request.getType() == SetMonth)DisplayBuffer[1]|=DISP_8;
		if(BlinkOn == 0 &&request.getType() == DebouncingDate)
		{
			DisplayBuffer[2]|=DISP_8;
			DisplayBuffer[1]|=DISP_8;
		}
		break;
	case SetYear:
		DisplayBuffer[3] = DISP_3;
		DisplayBuffer[2] = DISP_6;
		DisplayBuffer[1] = numToPort[request[2] / 10];
		DisplayBuffer[0] = numToPort[request[2] % 10];
		if(BlinkOn == 0)DisplayBuffer[2]=DISP_8|DISP_1;
		break;
		break;
	case showStepCounter: {
		uint16_t steps = request[2] / 100;
		DisplayBuffer[3] = DISP_7;
		DisplayBuffer[2] = numToPort[(steps / 100) % 16];
		DisplayBuffer[1] = numToPort[((steps) / 10) % 10];
		DisplayBuffer[0] = numToPort[((steps) % 10)];
		break;
	}
	case ShowStepsHistory: {
		uint16_t steps = request[1] / 100;

		DisplayBuffer[3] = DISP_8 | numToPort[request[0] % 8];
		DisplayBuffer[2] = numToPort[(steps / 100) % 16];
		DisplayBuffer[1] = numToPort[((steps) / 10) % 10];
		DisplayBuffer[0] = numToPort[((steps) % 10)];
		break;
	}
	case ShowUBatt: {
		uint16_t Ubatt_10mv = request[0] / 10;

		DisplayBuffer[3] = DISP_4;
		DisplayBuffer[2] = numToPort[(Ubatt_10mv / 100) % 16];
		DisplayBuffer[1] = numToPort[((Ubatt_10mv) / 10) % 10];
		DisplayBuffer[0] = numToPort[((Ubatt_10mv) % 10)];
		break;
	}
	case showError: {
		uint16_t error = request[0];
		uint16_t data = request[1];
		DisplayBuffer[3] = numToPort[0x0E];
		DisplayBuffer[2] = numToPort[error & 0x0F];
		DisplayBuffer[1] = numToPort[(data / 10) % 10];
		DisplayBuffer[0] = numToPort[data % 10];
		break;
	}
	case Raw_Output: {
		DisplayBuffer[3] = numToPort[request[0] & 0x0F];
		DisplayBuffer[2] = numToPort[request[1] & 0x0F];
		DisplayBuffer[1] = numToPort[request[2] & 0x0F];
		DisplayBuffer[0] = numToPort[request[3] & 0x0F];
		break;
	}
	default:
		DisplayBuffer[0] = DISP_F;
		DisplayBuffer[1] = DISP_F;
		DisplayBuffer[2] = DISP_F;
		DisplayBuffer[3] = DISP_F;
		break;
	}
	showLEDs(DisplayBuffer, 1, Brightness);
	while (request.getType() == showError)
		showLEDs(DisplayBuffer, 1, Brightness);

	DisplayRequest empty_dr;
	request = empty_dr;
}

