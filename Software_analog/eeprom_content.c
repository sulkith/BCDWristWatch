#include <avr/eeprom.h>
#include "types.h"

//correction_t EEMEM correction_EEP={0,2,0,0};
correction_t EEMEM correction_EEP={0,1,14,0};
uint16_t EEMEM tempCorrectionOffset_EEP = 345;
//TempLogEntry_t EEMEM LogEntries_EEP[24*14];
