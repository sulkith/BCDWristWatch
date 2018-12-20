typedef struct{
	int8_t everyMinute;
	int8_t everyHour;
	int8_t everyDay;
	int8_t everyMonth;
} correction_t;

correction_t EEMEM correction_EEP={0,2,0,0};
