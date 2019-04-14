#include <avr/eeprom.h>
#include "settings.h"
#include "EEPM.h"

EEPM::EEPM()
{
  eep.eepVers = eepVersion;
  eep.corrEveryMinute = iniCorrEveryMinute;
  eep.corrEveryHour = iniCorrEveryHour;
  eep.corrEveryDay = iniCorrEveryDay;
  eep.corrEveryMonth = iniCorrEveryMonth;
  eep.activatedFeatures = iniActivatedFeatures;
  eep.tempOffset = iniTempOffset;
}
