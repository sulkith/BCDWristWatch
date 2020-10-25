/*
 * settings.cpp
 *
 *  Created on: 25.10.2020
 *      Author: sulkith
 */
#include "settings.hpp"
WatchSettings_t SettingsDefault =
{
	false,/*bool enableSeconds;*/
	true,/*bool enableDate;*/
	true,/*bool enableStepHistory;*/
	50,/*int16_t debounceSetTime;*/
	50/*int16_t debounceStepsHistory;*/
};

WatchSettings_t SettingsAnalogV1 =
{
	false,/*bool enableSeconds;*/
	false,/*bool enableDate;*/
	true,/*bool enableStepHistory;*/
	20,/*int16_t debounceSetTime;*/
	50/*int16_t debounceStepsHistory;*/
};

WatchSettings_t SettingsBinaryV1 =
{
	true,/*bool enableSeconds;*/
	true,/*bool enableDate;*/
	true,/*bool enableStepHistory;*/
	50,/*int16_t debounceSetTime;*/
	100/*int16_t debounceStepsHistory;*/
};
