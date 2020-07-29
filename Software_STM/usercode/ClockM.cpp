#include <ClockM.hpp>
#include "main.h"
//#include "DisplayManager.hpp" //only for testing

ClockM ClockM::instance;
RTC_TimeTypeDef time_cs = { 0 };
RTC_DateTypeDef date_cs = { 0 };

extern RTC_HandleTypeDef hrtc;

const uint8_t DaysOfMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

ClockM::ClockM() {
}
void ClockM::updateLocalVars() {
	HAL_RTC_GetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
}
void ClockM::commitLocalVars() {
	HAL_RTC_SetTime(&hrtc, &time_cs, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &date_cs, RTC_FORMAT_BIN);
}
uint8_t ClockM::advanceMinute() {
	updateLocalVars();
	time_cs.Minutes = (time_cs.Minutes + 1) % 60;
	time_cs.Seconds = 0;
	commitLocalVars();
	return time_cs.Minutes;
}
uint8_t ClockM::advanceHour() {
	updateLocalVars();
	time_cs.Hours = (time_cs.Hours + 1) % 24;
	time_cs.Seconds = 0;
	commitLocalVars();
	return time_cs.Hours;
}
uint8_t ClockM::decreaseMinute() {
	updateLocalVars();
	if (time_cs.Minutes == 0)
		time_cs.Minutes = 60;
	time_cs.Minutes--;
	time_cs.Seconds = 0;
	commitLocalVars();
	return time_cs.Minutes;
}
uint8_t ClockM::decreaseHour() {
	updateLocalVars();
	if (time_cs.Hours == 0)
		time_cs.Hours = 24;
	time_cs.Hours--;
	time_cs.Seconds = 0;
	commitLocalVars();
	return time_cs.Hours;
}
uint8_t ClockM::advanceMonth()
{
	updateLocalVars();
	date_cs.Month = (date_cs.Month)%12+1;
	commitLocalVars();
	return date_cs.Month;
}
uint8_t ClockM::decreaseMonth() {
	updateLocalVars();
	if (date_cs.Month == 0)
		date_cs.Month = 24;
	date_cs.Month--;
	commitLocalVars();
	return time_cs.Hours;
}
uint8_t isLeapYear(uint8_t year)
{
	if(year == 0)
		return 0;
	if(year%4==0)
		return 1;
	return 0;
}
uint8_t ClockM::advanceDay()
{
	updateLocalVars();
	if(date_cs.Month == 2 && isLeapYear(date_cs.Year))
	{
		date_cs.Date = (date_cs.Date)%29+1;
	}
	else
	{
		date_cs.Date = (date_cs.Date)%DaysOfMonth[date_cs.Month-1]+1;
	}
	commitLocalVars();
	return date_cs.Date;
}
uint8_t ClockM::decreaseDay() {
	updateLocalVars();
	if(date_cs.Date == 1)
	{
		if(date_cs.Month == 2 && isLeapYear(date_cs.Year))
		{
			date_cs.Date = 29;
		}
		else
		{
			date_cs.Date = DaysOfMonth[date_cs.Month-1];
		}
	}
	else
	{
		date_cs.Date--;
	}
	commitLocalVars();
	return time_cs.Hours;
}
uint8_t ClockM::advanceYear()
{
	updateLocalVars();
	date_cs.Year++;
	date_cs.Year = date_cs.Year % 100;
	commitLocalVars();
	return date_cs.Year;
}
uint8_t ClockM::decreaseYear()
{
	updateLocalVars();
	if(date_cs.Year == 0)
		date_cs.Year = 99;
	else
		date_cs.Year--;
	commitLocalVars();
	return date_cs.Year;
}


void ClockM::clearSeconds() {
	updateLocalVars();
	time_cs.Seconds = 0;
	time_cs.SubSeconds = 0;
	commitLocalVars();
}
