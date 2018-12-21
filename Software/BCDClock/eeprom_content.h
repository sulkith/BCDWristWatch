typedef struct{
	int8_t everyMinute;
	int8_t everyHour;
	int8_t everyDay;
	int8_t everyMonth;
} correction_t;

//correction_t EEMEM correction_EEP={0,2,0,0};
correction_t EEMEM correction_EEP={0,1,11,13}; //remaining Difference per Day: 0.137259516460902 PCB2
