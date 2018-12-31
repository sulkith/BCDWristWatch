#ifndef __INCLUDE_SETTINGS_H__
#define __INCLUDE_SETTINGS_H__
#include "constants.h"

const uint8_t shifting[] =
{
	DISP_0,
	DISP_0,
	DISP_0,
	DISP_1,
	DISP_3,
	DISP_7,
	DISP_F,
	DISP_E,
	DISP_C,
	DISP_8,
	DISP_0,
	DISP_0,
	DISP_0
};

const uint16_t ontime_short=5000;
const uint16_t ontime_long=30000;
const uint16_t Button_debounceTime=30;
const uint8_t LED_Brightness = 120;

#endif
