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
 extern void MX_GPIO_Init();
 extern void MX_RTC_Init();
 extern void MX_SPI1_Init();

 #ifdef __cplusplus
 }
#endif

#endif /* USERMAIN_HPP_ */
