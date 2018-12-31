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
}  time;
enum state_t
{
	idle = 0x00,
	read_temperature = 0x01,
	display_on = 0x10,
	set_hour = 0x20,
	set_minute = 0x21
};

#endif
