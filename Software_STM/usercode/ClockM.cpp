#include <ClockM.hpp>
#include "main.h"
//#include "DisplayManager.hpp" //only for testing

ClockM ClockM::instance;
RTC_TimeTypeDef time_cs = { 0 };
RTC_DateTypeDef date_cs = { 0 };

extern RTC_HandleTypeDef hrtc;

ClockM::ClockM() {
}
void ClockM::updateLocalVars() {
	HAL_RTC_GetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	t.hour = time_cs.Hours;
	t.minute = time_cs.Minutes;
	t.second = time_cs.Seconds;
}
uint8_t ClockM::advanceMinute() {
	HAL_RTC_GetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	time_cs.Minutes = (time_cs.Minutes + 1) % 60;
	time_cs.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	return time_cs.Minutes;
}
uint8_t ClockM::advanceHour() {
	HAL_RTC_GetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	time_cs.Hours = (time_cs.Hours + 1) % 24;
	time_cs.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	return time_cs.Hours;
}
uint8_t ClockM::decreaseMinute() {
	HAL_RTC_GetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	if (time_cs.Minutes == 0)
		time_cs.Minutes = 60;
	time_cs.Minutes--;
	time_cs.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	return time_cs.Minutes;
}
uint8_t ClockM::decreaseHour() {
	HAL_RTC_GetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	if (time_cs.Hours == 0)
		time_cs.Hours = 24;
	time_cs.Hours--;
	time_cs.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
	return time_cs.Hours;
}
void ClockM::clearSeconds()
{
	HAL_RTC_GetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
		time_cs.Seconds = 0;
		time_cs.SubSeconds = 0;
		HAL_RTC_SetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
}
