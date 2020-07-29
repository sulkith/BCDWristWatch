#ifndef __CLOCKM_HEADER_INCLUDE__
#define __CLOCKM_HEADER_INCLUDE__

#include "main.h"

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t steps;
} time_t;
class ClockM {
	time_t t;
	RTC_TimeTypeDef time_cs = { 0 };
	RTC_DateTypeDef date_cs = { 0 };
	uint32_t ctr;
	bool hour_changed = 0;
	static ClockM instance;
	ClockM();
	void updateLocalVars();
	void commitLocalVars();
public:
	uint8_t getSecond() {
		updateLocalVars();
		return time_cs.Seconds;
	}
	uint8_t getMinute() {
		updateLocalVars();
		return time_cs.Minutes;
	}
	uint8_t getHour() {
		updateLocalVars();
		return time_cs.Hours;
	}
	uint8_t getDay() {
		updateLocalVars();
		return date_cs.Date;
	}
	uint8_t getMonth() {
		updateLocalVars();
		return date_cs.Month;
	}
	uint8_t getYear() {
			updateLocalVars();
			return date_cs.Year;
		}
	bool isHourChanged() {
		return hour_changed;
	}
	void setHourChanged(bool changed) {
		hour_changed = changed;
	}
	uint32_t getCtr() {
		return HAL_GetTick() / 1000;
	}
	uint8_t advanceMinute();
	uint8_t advanceHour();
	uint8_t decreaseMinute();
	uint8_t decreaseHour();
	uint8_t advanceDay();
	uint8_t decreaseDay();
	uint8_t advanceMonth();
	uint8_t decreaseMonth();
	uint8_t advanceYear();
	uint8_t decreaseYear();
	void clearSeconds();

	static ClockM& getInstance() {
		return instance;
	}
	ClockM(ClockM const&) = delete;
	void operator=(ClockM const&) = delete;

};
#endif //__CLOCKM_HEADER_INCLUDE__
