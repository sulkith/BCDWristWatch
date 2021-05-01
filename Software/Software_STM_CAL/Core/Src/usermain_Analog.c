/*
 * usermain.c
 *
 *  Created on: 16.10.2020
 *      Author: sulkith
 */

#include "main.h"
extern RTC_HandleTypeDef hrtc;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

#define Undefined 0xFFFFUL
#define Binary_STM_V1 0x0100UL
#define Analog_STM_V1 0x0200UL
const uint32_t WatchVariant = Analog_STM_V1;

const uint32_t GPIOA_MODER_Minutes[]={0xFFFF5FFF,0xFFFF7FFF,0xFFFF7FFF,0xFFFF77FF,0xFFFFF7FF,0xFFFFF7FF,0xFFFFD7FF,0xFFFFFD7F,0xFFFFFF5F,0xFFFFFDDF,0xFFFFFDDF,0xFFFFFFD7,0xFFFFFDF7,0xFFFFFDF7,0xFFFFFF77,0xFF7FFFFF,0x7F7FFFFF,0x7FFFFFFF,0x7FFFFFFF,0x7DFFFFFF,0xFDFFFFFF,0xFDFFFFFF,0xFFDDFFFF,0xFFF5FFFF,0xFFD7FFFF,0xFFD7FFFF,0xFFF7FFFF,0xFFDFFFFF,0xFFFFDFFF,0xFFFFDFFF,};const uint32_t GPIOB_MODER_Minutes[]={0xFFFFFFFF,0xFFFFFFFD,0xFFFFFFFD,0xFFFFFFFF,0xFFFFFFFD,0xFFFFFFFD,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFF7FFF,0xFFFFFFFF,0xFFFF7FFF,0xFFFF7FFF,0xFFFFFFFF,0xFFFF7FFF,0xFFFF7FFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFF7,0xFFFFFFF7,0xFFFFFFFD,0xFFFFFFFD,};const uint32_t GPIOA_MODER_Hours[]={0xFFFF5FFF,0xFFFF77FF,0xFFFFD7FF,0xFFFFFF5F,0xFFFFFFD7,0xFFFFFF77,0x7F7FFFFF,0x7DFFFFFF,0xFD7FFFFF,0xFFF5FFFF,0xFFF7FFFF,0xFFFDFFFF,};const uint32_t GPIOB_MODER_Hours[]={0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFF7,0xFFFFFFF7,};const uint16_t GPIOA_ODR_Minutes[]={0x40,0x0,0x80,0x80,0x20,0x0,0x20,0x8,0x8,0x10,0x4,0x4,0x2,0x10,0x2,0x800,0x800,0x0,0x8000,0x8000,0x1000,0x0,0x100,0x100,0x400,0x200,0x200,0x0,0x0,0x40,};const uint16_t GPIOB_ODR_Minutes[]={0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x2,0x1,0x0,};const uint16_t GPIOA_ODR_Hours[]={0x80,0x20,0x40,0x4,0x2,0x8,0x8000,0x1000,0x800,0x200,0x0,0x100,};const uint16_t GPIOB_ODR_Hours[]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,};


uint8_t Buffer[4] = { 0 };

void clearLED_Analog() {
	//Clear All LEDs
	GPIOA->MODER = 0xFFFFFFFF;
	GPIOB->MODER = 0xFFFFFFFF;
	GPIOA->ODR = 0x0000;
	GPIOB->ODR = 0x0000;
}

void Analog_showLED(uint8_t LED) {
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

void Analog_showLEDs(uint8_t DisplayBuffer[], uint16_t duration) {
	//Show LED 0-2 sequentially and LED 3 and 4 together, the user needs to make sure that LED3 and LED4 are compatible to each other
	for (uint16_t j = 0; j < duration; ++j) {
		for (uint8_t i = 0; i < 3; ++i) {
			Analog_showLED(DisplayBuffer[i]);
			DWT_Delay_us(255);
			clearLED_Analog();
		}
		Analog_showLED(DisplayBuffer[3]);
	}
	clearLED_Analog();
}

void init_IOs() {
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	//Lock PA13 and PA14 (SWD-Ports)
	const uint32_t GPIOA_Lock = (1 << 13) + (1 << 14)+ (1 << 0);
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
}
void showError()
{

	Buffer[3] = 15;
	Buffer[2] = 14;
	Buffer[1] = 15;
	Buffer[0] = 16;
	Analog_showLEDs(Buffer, 500);
}
void showFinish()
{
	Buffer[3] = 100;
	Buffer[2] = 103;
	Buffer[1] = 106;
	Buffer[0] = 109;
	Analog_showLEDs(Buffer, 500);
}
void testLEDsAnalog()
{
	for(uint8_t j = 0;j<20;j++)
	{
		for(uint8_t i = 0;i<31;i++)
		{
			Buffer[3] = i;
			Buffer[2] = i;
			Buffer[1] = i;
			Buffer[0] = i;
			Analog_showLEDs(Buffer, 1);
		}
		for(uint8_t i = 100;i<113;i++)
		{
			Buffer[3] = i;
			Buffer[2] = i;
			Buffer[1] = i;
			Buffer[0] = i;
			Analog_showLEDs(Buffer, 1);
		}
	}
}
void testLEDs()
{
	for(uint8_t j = 0;j<20;j++)
	{
		for(uint8_t i = 0;i<31;i++)
		{
			Buffer[3] = i;
			Buffer[2] = i;
			Buffer[1] = i;
			Buffer[0] = i;
			Analog_showLEDs(Buffer, 1);
		}
		for(uint8_t i = 100;i<113;i++)
		{
			Buffer[3] = i;
			Buffer[2] = i;
			Buffer[1] = i;
			Buffer[0] = i;
			Analog_showLEDs(Buffer, 1);
		}
	}
}
void showRemainingTime(uint16_t TimeToGo)
{
	uint16_t led = (TimeToGo*30)/secondsToMeasure;
	Buffer[3] = led;
	Buffer[2] = led;
	Buffer[1] = led;
	Buffer[0] = led;
	Analog_showLEDs(Buffer, 500);
}
