/*
 * usermain.cpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */
#include "usermain.hpp"
#include "ClockM.hpp"
#include "EEPM.hpp"
#include "DisplayManager.hpp"
#include "DisplayRequest.hpp"

#include "UserInterface.hpp"
#include "HAL.hpp"
#include "SleepM.hpp"

#include "STM32L4_BCDDisplayManager.hpp"
#include "STM32L4_HAL.hpp"
#include "GForceUI.hpp"

extern RTC_HandleTypeDef hrtc;


DisplayManager *dman;
STM32L4_HAL stm_hal;
STM32L4_BCDDisplayManager BCDdm;
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
void usermain_init() {

	  SystemClock_Config();

	  MX_RTC_Init();
	  // Moved to HAL: MX_SPI1_Init();
	  Watch_Type_t wt = Binary_v1;

	  //hal = &stm_hal;
	  DWT_Delay_Init();

	  //TODO Code to distinguish between analog and Binary watch Version.
	  if(wt == Binary_v1)
	  {
		  dman = &BCDdm;
	  }
	  else
	  {
		  //TODO Init Analog Dman
		  dman = &BCDdm;
	  }

	  stm_hal.setAxisMappingVariant(wt);
	  stm_hal.setDisplayManager(dman);
	  dman->init();
	  stm_hal.HAL_init();


	  gfui.init(dman, &stm_hal, &stm_hal);
	  SleepM::getInstance()->setHal(&stm_hal);

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
	dman->show();
	SleepM::getInstance()->cyclic();
	stm_hal.HAL_cyclic();
}
