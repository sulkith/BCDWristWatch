#ifndef __INCLUDE_TYPES_H__
#define __INCLUDE_TYPES_H__

typedef struct{
	uint8_t everyMinute;
	uint8_t everyHour;
	uint8_t everyDay;
	uint8_t everyMonth;
} correction_t;

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint16_t steps;
}  time;
typedef struct{
	uint16_t offset;
	uint16_t correction[50];
} tempCorrection_t;
typedef struct{
	uint8_t datetime;
	uint8_t temp;
} TempLogEntry_t;
enum state_t
{
	idle = 0x00,
	read_temperature = 0x01,
	display_on = 0x10,
	set_hour = 0x20,
	set_minute = 0x21,
	set_eep_min_correction = 0x31,
	set_eep_hour_correction = 0x32,
	set_eep_day_correction = 0x33,
	set_eep_month_correction = 0x34,
	save_eeprom = 0x3F
};

#endif
