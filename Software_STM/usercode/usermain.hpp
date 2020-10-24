/*
 * usermain.hpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */

#ifndef USERMAIN_HPP_
#define USERMAIN_HPP_

#include "stm32l4xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

 extern void usermain_init();
 extern void usermain_loop();
 extern void RTC_Event_Callback();

 extern void SystemClock_Config();
 //extern void MX_GPIO_Init();
 extern void MX_ADC1_Init();
 extern void MX_RTC_Init();
 extern void MX_SPI1_Init();

 #ifdef __cplusplus
 }
#endif

#define WatchVersCodeAddress (FLASH_BASE + FLASH_SIZE - FLASH_PAGE_SIZE + 0x10)

#define Undefined 0xFFFFUL
#define Binary_STM_V1 0x0100UL
#define Analog_STM_V1 0x0200UL
//const uint32_t WatchVariant = Binary_STM_V1;

#define COMPILE_ANALOG_V1
#define COMPILE_BINARY_V1

#endif /* USERMAIN_HPP_ */
