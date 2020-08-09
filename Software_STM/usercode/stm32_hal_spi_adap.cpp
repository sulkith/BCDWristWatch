/*
 * stm32_hal_spi_adap.cpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */
#include "main.h"
#include "libs/Bosch_BMA456/spi_adap.h"
#include "libs/Bosch_BMA456/bus_adap.h"
extern SPI_HandleTypeDef hspi1;

//#ifndef SPI_BUFFER_LENGTH
#define SPI_BUFFER_LENGTH 132
//#endif

extern SPI_HandleTypeDef hspi1;

const uint32_t SPI_TIMEOUT = 1000;
uint8_t buffer[SPI_BUFFER_LENGTH];

/**
 * https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
 * @brief  This function provides a delay (in microseconds)
 * @param  microseconds: delay in microseconds
 */
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

void spi_init()
{
	//Nothing to do here
	HAL_StatusTypeDef ret = HAL_SPI_Transmit(&hspi1, buffer, 1, SPI_TIMEOUT);
	UNUSED(ret);
}
void spi_write(uint8_t* data, uint8_t length, void (*callback)(uint8_t *))
{
	HAL_StatusTypeDef ret = HAL_SPI_Transmit(&hspi1, data, length, SPI_TIMEOUT);
	UNUSED(ret);
	return;
}
void spi_read(uint8_t length, void (*callback)(uint8_t *))
{
	HAL_StatusTypeDef ret = HAL_SPI_Receive(&hspi1, buffer, length, SPI_TIMEOUT);
	UNUSED(ret);
	(*callback)(buffer);
};
uint8_t *spi_wait()
{
	//Nothing to do here, since we use the syncronus version
	return buffer;
}
void _delay_us(uint32_t time)
{
	DWT_Delay_us(time);
}
