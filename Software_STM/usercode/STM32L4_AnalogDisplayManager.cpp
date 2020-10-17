/*
 * STM32L4_AnalogDisplayManager.cpp
 *
 *  Created on: Oct 04, 2020
 *      Author: sulkith
 */

#include "STM32L4_AnalogDisplayManager.hpp"
#include "SleepM.hpp"
#include "main.h"
#include "pins.hpp"
const uint32_t GPIOA_MODER_Minutes[]={0xFFFF5FFF,0xFFFF7FFF,0xFFFF7FFF,0xFFFF77FF,0xFFFFF7FF,0xFFFFF7FF,0xFFFFD7FF,0xFFFFFD7F,0xFFFFFF5F,0xFFFFFDDF,0xFFFFFDDF,0xFFFFFFD7,0xFFFFFDF7,0xFFFFFDF7,0xFFFFFF77,0xFF7FFFFF,0x7F7FFFFF,0x7FFFFFFF,0x7FFFFFFF,0x7DFFFFFF,0xFDFFFFFF,0xFDFFFFFF,0xFFDDFFFF,0xFFF5FFFF,0xFFD7FFFF,0xFFD7FFFF,0xFFF7FFFF,0xFFDFFFFF,0xFFFFDFFF,0xFFFFDFFF,};const uint32_t GPIOB_MODER_Minutes[]={0xFFFFFFFF,0xFFFFFFFD,0xFFFFFFFD,0xFFFFFFFF,0xFFFFFFFD,0xFFFFFFFD,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFF7FFF,0xFFFFFFFF,0xFFFF7FFF,0xFFFF7FFF,0xFFFFFFFF,0xFFFF7FFF,0xFFFF7FFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFF7,0xFFFFFFF7,0xFFFFFFFD,0xFFFFFFFD,};const uint32_t GPIOA_MODER_Hours[]={0xFFFF5FFF,0xFFFF77FF,0xFFFFD7FF,0xFFFFFF5F,0xFFFFFFD7,0xFFFFFF77,0x7F7FFFFF,0x7DFFFFFF,0xFD7FFFFF,0xFFF5FFFF,0xFFF7FFFF,0xFFFDFFFF,};const uint32_t GPIOB_MODER_Hours[]={0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFF7,0xFFFFFFF7,};const uint16_t GPIOA_ODR_Minutes[]={0x40,0x0,0x80,0x80,0x20,0x0,0x20,0x8,0x8,0x10,0x4,0x4,0x2,0x10,0x2,0x800,0x800,0x0,0x8000,0x8000,0x1000,0x0,0x100,0x100,0x400,0x200,0x200,0x0,0x0,0x40,};const uint16_t GPIOB_ODR_Minutes[]={0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x2,0x1,0x0,};const uint16_t GPIOA_ODR_Hours[]={0x80,0x20,0x40,0x4,0x2,0x8,0x8000,0x1000,0x800,0x200,0x0,0x100,};const uint16_t GPIOB_ODR_Hours[]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,};
void clearLED_Analog();

//This function is intended to show two LEDs which are in different Charlieplex matrices.
void showLED_Analog_Dual(uint8_t LED1, uint8_t LED2) {
	if (LED1 == 255) {
		clearLED_Analog(); //if LED1 is 255 nothing should be shown
		return;
	}
	//AND Mode Register for GPIOA and GPIOB
	uint32_t MODERA =
			(LED1 >= 100) ?
					GPIOA_MODER_Hours[(LED1 - 100)%12] : GPIOA_MODER_Minutes[LED1%30];
	if (LED2 != 255)
		MODERA &=
				(LED2 >= 100) ?
						GPIOA_MODER_Hours[(LED2 - 100)%12] :
						GPIOA_MODER_Minutes[LED2%30];
	uint32_t MODERB =
			(LED1 >= 100) ?
					GPIOB_MODER_Hours[(LED1 - 100)%12] : GPIOB_MODER_Minutes[LED1%30];
	if (LED2 != 255)
		MODERB &=
				(LED2 >= 100) ?
						GPIOB_MODER_Hours[(LED2 - 100)%12] :
						GPIOB_MODER_Minutes[LED2%30];

	//OR Output Data Register for GPIOA and GPIOB
	uint32_t ODRA =
			(LED1 >= 100) ?
					GPIOA_ODR_Hours[(LED1 - 100)%12] : GPIOA_ODR_Minutes[LED1%30];
	if (LED2 != 255)
		ODRA |= (LED2 >= 100) ?
				GPIOA_ODR_Hours[(LED2 - 100)%12] : GPIOA_ODR_Minutes[LED2%30];
	uint32_t ODRB =
			(LED1 >= 100) ?
					GPIOB_ODR_Hours[(LED1 - 100)%12] : GPIOB_ODR_Minutes[LED1%30];
	if (LED2 != 255)
		ODRB |= (LED2 >= 100) ?
				GPIOB_ODR_Hours[(LED2 - 100)%12] : GPIOB_ODR_Minutes[LED2%30];

	//Show the LEDs
	GPIOA->MODER = MODERA;
	GPIOB->MODER = MODERB;
	GPIOA->ODR = ODRA;
	GPIOB->ODR = ODRB;
}
void showLED_Analog(uint8_t LED) {
	if (LED == 255) {
		clearLED_Analog();
	} else if (LED >= 100) //hour
			{
		uint8_t hr_index = (LED - 100) % 12;
		GPIOA->MODER = GPIOA_MODER_Hours[hr_index];
		GPIOB->MODER = GPIOB_MODER_Hours[hr_index];
		GPIOA->ODR = GPIOA_ODR_Hours[hr_index];
		GPIOB->ODR = GPIOB_ODR_Hours[hr_index];
	} else {
		uint8_t min_index = (LED % 30);
		GPIOA->MODER = GPIOA_MODER_Minutes[min_index];
		GPIOB->MODER = GPIOB_MODER_Minutes[min_index];
		GPIOA->ODR = GPIOA_ODR_Minutes[min_index];
		GPIOB->ODR = GPIOB_ODR_Minutes[min_index];
	}
}
void clearLED_Analog() {
	//Clear All LEDs
	GPIOA->MODER = 0xFFFFFFFF;
	GPIOB->MODER = 0xFFFFFFFF;
	GPIOA->ODR = 0x0000;
	GPIOB->ODR = 0x0000;
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
void showLEDs_Analog(uint8_t DisplayBuffer[], uint16_t duration, uint8_t perc) {
	//Show LED 0-2 sequentially and LED 3 and 4 together, the user needs to make sure that LED3 and LED4 are compatible to each other
	for (uint16_t j = 0; j < duration; ++j) {
		for (uint8_t i = 0; i < 3; ++i) {
			showLED_Analog(DisplayBuffer[i]);
			DWT_Delay_us(255);
			clearLED_Analog();
		}
		showLED_Analog_Dual(DisplayBuffer[3], DisplayBuffer[4]);
	}
	clearLED_Analog();
}
void STM32L4_AnalogDisplayManager::executeSleepSubscription() {

}
void STM32L4_AnalogDisplayManager::lockPorts()
{
	//Lock PA13 and PA14 (SWD-Ports)
	const uint32_t GPIOA_Lock = (1 << 13) + (1 << 14);
	//Lock PB3, PB4, PB5, PB6 SPI Port for the BMA
	const uint32_t GPIOB_Lock = (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6);
	volatile uint32_t LockReadBack = 0;

	//Lock Sequence for Port A
	GPIOA->LCKR = 0x10000 + GPIOA_Lock;
	GPIOA->LCKR = 0x00000 + GPIOA_Lock;
	GPIOA->LCKR = 0x10000 + GPIOA_Lock;
	LockReadBack = GPIOA->LCKR;

	//Lock Sequence for Port A
	GPIOB->LCKR = 0x10000 + GPIOB_Lock;
	GPIOB->LCKR = 0x00000 + GPIOB_Lock;
	GPIOB->LCKR = 0x10000 + GPIOB_Lock;
	LockReadBack = GPIOB->LCKR;

	//Initialize Ports so all LEDs are off
	clearLED_Analog();
}
void STM32L4_AnalogDisplayManager::init() {
	//Start GPIO Clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	SleepM::getInstance()->subscribe(this);
}
#define MIN(a,b) (a<b?a:b)
void STM32L4_AnalogDisplayManager::show() {
	uint8_t BlinkOn = (HAL_GetTick() / 150) % 2;
	uint8_t DisplayBuffer[5] = { 255, 255, 255, 255, 255 };
	switch (request.getType()) {
	case Empty:
		return; // no need to stay awake for long
	case FadeIn:
		for (uint8_t i = 0; i < 60; ++i) {
			uint8_t minutes = MIN(i, request[1]);
			uint8_t hour = MIN(((i + 3) / 5),request[0]%12) + 100;
			DisplayBuffer[0] = minutes/2;
			if (minutes % 2 == 1)
				DisplayBuffer[1] = minutes / 2 + 1;
			else
				DisplayBuffer[1] = 255;
			DisplayBuffer[2] = hour;
			DisplayBuffer[3] = 255;
			showLEDs_Analog(DisplayBuffer, 10, 255);
			DWT_Delay_us(220);//TODO: Trim
		}
		return;
		break;
	case SetHour:
		DisplayBuffer[2] = request[0] + 100;
		if (BlinkOn == 0) {
			if(request[0] > 11)
			{
				DisplayBuffer[0] = 5;
			}
			DisplayBuffer[3] = 0;
			DisplayBuffer[4] = 15;
		}
		break;
	case SetMinute:
		DisplayBuffer[0] = request[1] / 2;
		if (request[1] % 2 == 1)
			DisplayBuffer[1] = request[1] / 2 + 1;
		else
			DisplayBuffer[1] = 255;
		DisplayBuffer[2] = 106;
		if (BlinkOn == 0)
			DisplayBuffer[3] = 100;
		break;
	case Time:
	case Debouncing:
	case Time_s:
		DisplayBuffer[0] = request[1] / 2;
		if (request[1] % 2 == 1)
			DisplayBuffer[1] = request[1] / 2 + 1;
		else
			DisplayBuffer[1] = 255;
		DisplayBuffer[2] = request[0] + 100;
		DisplayBuffer[3] = 255;

		if (BlinkOn == 0 && request.getType() == Debouncing) {
			DisplayBuffer[3] = 100;
			DisplayBuffer[4] = 106;
		}
		break;
	case Date:
	case SetDay:
	case SetMonth:
	case DebouncingDate:
		DisplayBuffer[0] = request[0] / 2;
		if (request[0] % 2 == 1)
			DisplayBuffer[1] = request[0] / 2 + 1;
		else
			DisplayBuffer[1] = 255;
		DisplayBuffer[2] = request[1] + 100;
		DisplayBuffer[3] = 20;
		DisplayBuffer[4] = 25;
		if (BlinkOn == 1 && request.getType() == SetDay) {
			DisplayBuffer[3] = 255;
		}
		if (BlinkOn == 1 && request.getType() == SetMonth) {
			DisplayBuffer[4] = 255;
		}
		if (BlinkOn == 1 && request.getType() == DebouncingDate) {
			DisplayBuffer[3] = 255;
			DisplayBuffer[4] = 255;
		}
		break;
	case SetYear:
		DisplayBuffer[0] = request[2] % 10;
		DisplayBuffer[1] = (request[2] / 10) + 100;
		if (BlinkOn == 0) {
			DisplayBuffer[3] = 15;
			DisplayBuffer[4] = 25;
		}
		break;
	case showStepCounter:
	case ShowStepsHistory:
	{
		uint32_t steps = request[2];
		DisplayBuffer[0] = (steps % 15000) / 500;
		DisplayBuffer[1] = (steps / 15000)+100;
		DisplayBuffer[2] = 111;
		if (request.getType() == ShowStepsHistory) {
			DisplayBuffer[3] = request[0] + 100 + 4;
		}
		break;
	}
	case ShowUBatt:
	{
		uint16_t Ubatt_50mv = request[0] / 50;
		Ubatt_50mv -= 30;
		DisplayBuffer[0] = Ubatt_50mv;
		DisplayBuffer[1] = 100;
		DisplayBuffer[2] = 111;
		break;
	}
	case showError:
	{
		uint16_t error = request[0];
		DisplayBuffer[2] = error + 100;
		DisplayBuffer[0] = request[1] / 2;
		if (request[1] % 2 == 1)
			DisplayBuffer[1] = request[1] / 2 + 1;
		else
			DisplayBuffer[1] = 255;
		break;
	}
	case Raw_Output:
		DisplayBuffer[3] = request[0];
		DisplayBuffer[2] = request[1];
		DisplayBuffer[1] = request[2];
		DisplayBuffer[0] = request[3];
		break;
	default:
		DisplayBuffer[0] = 100;
		DisplayBuffer[1] = 103;
		DisplayBuffer[2] = 106;
		DisplayBuffer[3] = 109;
		break;
	}
	showLEDs_Analog(DisplayBuffer, 10, Brightness);
	while (request.getType() == showError)
		showLEDs_Analog(DisplayBuffer, 10, Brightness);

	DisplayRequest empty_dr;
	request = empty_dr;

}

