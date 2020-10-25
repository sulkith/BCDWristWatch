/*
 * usermain.cpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */
#include "usermain.hpp"
#include "ClockM.hpp"
#include "DisplayManager.hpp"
#include "DisplayRequest.hpp"

#include "UserInterface.hpp"
#include "HAL.hpp"
#include "SleepM.hpp"

#include "STM32L4_HAL.hpp"
#include "GForceUI.hpp"
#include "settings.hpp"

extern RTC_HandleTypeDef hrtc;

#ifdef COMPILE_BINARY_V1
#include "STM32L4_BCDDisplayManager.hpp"
#endif
#ifdef COMPILE_ANALOG_V1
#include "STM32L4_AnalogDisplayManager.hpp"
#endif


DisplayManager *dman;
STM32L4_HAL stm_hal;
#ifdef COMPILE_BINARY_V1
STM32L4_BCDDisplayManager BCDdm;
#endif
#ifdef COMPILE_ANALOG_V1
STM32L4_AnalogDisplayManager Adm;
#endif
GForceUI gfui;

void RTC_Event_Callback()
{
	stm_hal.updateSteps();
	stm_hal.pushNewSteps(stm_hal.getSteps());
	stm_hal.resetSteps();
}
uint32_t DWT_Delay_Init(void)
{
    /* Disable TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* Enable TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* Enable  clock cycle counter */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
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
    if(DWT->CYCCNT)
    {
       return 0; /*clock cycle counter started*/
    }
    else
    {
      return 1; /*clock cycle counter not started*/
    }
}
void SystemClock_Config_without_LSE(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}
inline void requestScreen(DisplayManager *dm, DisplayRequestType type,
		uint16_t d0, uint16_t d1 = 0, uint16_t d2 = 0, uint16_t d3 = 0,
		uint16_t d4 = 0) {
	uint16_t data[DisplayRequest::dataLength] = { 0 };
	data[0] = d0;
	data[1] = d1;
	data[2] = d2;
	data[3] = d3;
	data[4] = d4;
	DisplayRequest dr(type, data);
	dm->requestDisplay(dr);
}
//#define PROFILING(x) x
#ifndef PROFILING
	#define PROFILING(x)
#endif
void usermain_init() {
	  SystemClock_Config_without_LSE();
	  PROFILING(uint32_t time[20];)

	  // Moved to HAL: MX_RTC_Init();
	  // Moved to HAL: MX_SPI1_Init();
	  PROFILING(time[0] = HAL_GetTick();)
	  Watch_Type_t wt = Binary_v1;
	  uint64_t WatchVersCode = *((uint64_t*) WatchVersCodeAddress);
		if((WatchVersCode&0xFFFF) == Binary_STM_V1)
		{
		  wt = Binary_v1;
		  gfui.setSettingsPtr(&SettingsBinaryV1);
		}
		if((WatchVersCode&0xFFFF) == Analog_STM_V1)
		{
		  wt = Analog_v1;
		  gfui.setSettingsPtr(&SettingsAnalogV1);
		}
	  PROFILING(time[1] = HAL_GetTick();)
	  //hal = &stm_hal;
	  DWT_Delay_Init();

	  switch(wt)
	  {
#ifdef COMPILE_BINARY_V1
	  case Binary_v1:
		  dman = &BCDdm;
		  break;
#endif
#ifdef COMPILE_ANALOG_V1
	  case Analog_v1:
		  dman = &Adm;
#endif
	  	  break;
	  default:
		  Error_Handler();
	  }
	  PROFILING(time[2] = HAL_GetTick();)
	  stm_hal.setAxisMappingVariant(wt);
	  PROFILING(time[3] = HAL_GetTick();)
	  stm_hal.HAL_driverInit();
	  PROFILING(time[4] = HAL_GetTick();)
	  stm_hal.setDisplayManager(dman);
	  PROFILING(time[5] = HAL_GetTick();)
	  dman->init();
	  PROFILING(time[6] = HAL_GetTick();)
	  stm_hal.HAL_init();
	  PROFILING(time[7] = HAL_GetTick();)
	  dman->lockPorts();
	  PROFILING(time[8] = HAL_GetTick();)
	  stm_hal.HAL_cyclic();


	  gfui.init(dman, &stm_hal, &stm_hal);
	  PROFILING(time[9] = HAL_GetTick();)
	  SleepM::getInstance()->setHal(&stm_hal);
	  PROFILING(time[10] = HAL_GetTick();)

//	  while(1)
//	  {
//		  requestScreen(dman, Debouncing, 2, 3, 4);
//		  dman->show();
//	  }

	  if(stm_hal.HAL_getWakeupReason() < 0x10)//No Wakeup for Hourly Display
	  {
		  stm_hal.HAL_sleep();
	  }


}

void usermain_loop() {
	if(0)
	{
		uint16_t data[] = {12,48,0,0,0};
		data[0] = ClockM::getInstance().getHour();
		data[1] = ClockM::getInstance().getMinute();
		if(HAL_GPIO_ReadPin(BMA_INT_GPIO_Port, BMA_INT_Pin) == GPIO_PIN_SET)
		{
			data[0] = 21;
		}
		if(stm_hal.getTap()>0)
		{
			data[1] = 84;
		}
		DisplayRequest dr(Time, data);
		dman->requestDisplay(dr);
		dman->show();
	}
	gfui.cyclic();
	SleepM::getInstance()->cyclic();
	stm_hal.HAL_cyclic();
	dman->show();
}
