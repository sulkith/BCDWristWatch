#include <avr/eeprom.h>
#include "EEPM_AVR.h"
#include "settings.h"

eepBlock_t EEMEM settingsEEP =
{
  eepVersion,
  iniCorrEveryMinute,
  iniCorrEveryHour,
  iniCorrEveryDay,
  iniCorrEveryMonth,
  iniActivatedFeatures,
  iniTempOffset
};

const eepBlock_t iniSettings =
{
  eepVersion,
  iniCorrEveryMinute,
  iniCorrEveryHour,
  iniCorrEveryDay,
  iniCorrEveryMonth,
  iniActivatedFeatures,
  iniTempOffset
};
EEPM_AVR::EEPM_AVR()
{
  readEEP();
}
void EEPM_AVR::readEEP()
{
	eeprom_read_block(&eep,&settingsEEP,sizeof(eepBlock_t));
  if(eepVersion != eep.eepVers)
  {
    iniEEP();
  }
  eep_state = NORMAL;
}

void EEPM_AVR::saveEEP()
{
  eeprom_update_block(&eep,&settingsEEP,sizeof(eepBlock_t));
}
void EEPM_AVR::iniEEP()
{
  eeprom_update_block(&iniSettings,&settingsEEP,sizeof(eepBlock_t));
  readEEP();
}
void EEPM_AVR::executeSleepSubscription()
{
  if(eep_state == CHANGED)
  {
    saveEEP();
  }
}
