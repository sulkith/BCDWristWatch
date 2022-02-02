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
#define Binary_STM_V2_LIPO 0x0101UL
#define Analog_STM_V1 0x0200UL
const uint32_t WatchVariant = Binary_STM_V2_LIPO;

void BCD_showLEDs(uint16_t DisplayBuffer[], uint16_t duration) {
	const uint8_t perc = 250;
	for (uint16_t j = 0; j < duration; ++j)
		for (uint8_t i = 0; i < 4; ++i) {
			//PORTD=DisplayBuffer[i]|((0b00111100)&(~(1<<(2+i))));
			GPIOA->BSRR = DisplayBuffer[3 - i] | Column[i];
			DWT_Delay_us(perc);
			GPIOA->BSRR = mask_clear;
			DWT_Delay_us(250 - perc);
		}
}

void init_IOs() {
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
}
void showError()
{
	uint16_t Buffer[4] = { 0 };
	Buffer[3] = DISP_9;
	Buffer[2] = DISP_6;
	Buffer[1] = DISP_6;
	Buffer[0] = DISP_9;
	BCD_showLEDs(Buffer, 500);
}
void showFinish()
{
	uint16_t Buffer[4] = { 0 };
	Buffer[3] = DISP_2;
	Buffer[2] = DISP_1;
	Buffer[1] = DISP_2;
	Buffer[0] = DISP_4;
	BCD_showLEDs(Buffer, 500);
}
void testLEDs()
{
	uint16_t Buffer[4] = { 0 };
	Buffer[3] = DISP_F;
	Buffer[2] = DISP_F;
	Buffer[1] = DISP_F;
	Buffer[0] = DISP_F;
	BCD_showLEDs(Buffer, 500);
}
void showRemainingTime(uint16_t TimeToGo)
{
	uint16_t Buffer[4] = { 0 };
	Buffer[3] = DISP_C;
	Buffer[2] = numToPort[((TimeToGo) / 100) % 10];;
	Buffer[1] = numToPort[((TimeToGo) / 10) % 10];
	Buffer[0] = numToPort[(TimeToGo) % 10];
	BCD_showLEDs(Buffer, 500);
}
