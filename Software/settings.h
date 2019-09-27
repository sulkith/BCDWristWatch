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
//const uint16_t Button_debounceTime=30; //moved to  TwoButtonHAL
const uint8_t LED_Brightness = 120;

//SW V2
//incement this on every change to the EEPROM so the Values gets initialized again
const uint16_t eepVersion = 1;

const uint8_t iniCorrEveryMinute = 0;
const uint8_t iniCorrEveryHour = 1;
const uint8_t iniCorrEveryDay = 14;
const uint8_t iniCorrEveryMonth = 0;
const uint8_t iniActivatedFeatures = 0;
const uint16_t iniTempOffset = 345;

#endif
