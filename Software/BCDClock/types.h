#ifndef __INCLUDE_TYPES_H__
#define __INCLUDE_TYPES_H__

typedef struct{
	int8_t everyMinute;
	int8_t everyHour;
	int8_t everyDay;
	int8_t everyMonth;
} correction_t;

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
}time;
enum state_t
{
	hour,
	minute,
	idle
};

#endif
