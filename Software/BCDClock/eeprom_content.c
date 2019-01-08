#include <avr/eeprom.h>
#include "types.h"

//correction_t EEMEM correction_EEP={0,2,0,0};
correction_t EEMEM correction_EEP={0,1,14,0};
tempCorrection_t EEMEM tempCorrection_EEP={345,{	0,	1,	5,	10,	17,	26,	37,	50,	65,	82,	100,	121,	144,	168,	195,	223,	254,	286,	320,	356,	394,	434,	476,	520,	566,	614,	664,	715,	769,	824,	882,	941,	1003,	1066,	1131,	1198,	1267,	1338,	1411,	1486,	1563,	1641,	1722,	1805,	1889,	1976,	2064,	2154,	2247,	2341}
};
TempLogEntry_t EEMEM LogEntries_EEP[24*14];
