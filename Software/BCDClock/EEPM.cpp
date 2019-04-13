#include <avr/eeprom.h>
#include "EEPM.h"
#include "settings.h"

EEPM EEPM::instance;
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

void EEPM::readEEP()
{
	eeprom_read_block(&eep,&settingsEEP,sizeof(eepBlock_t));
  if(eepVersion != eep.eepVers)
  {
    iniEEP();
  }
  eep_state = NORMAL;
}

void EEPM::saveEEP()
{
  eeprom_update_block(&eep,&settingsEEP,sizeof(eepBlock_t));
}
void EEPM::iniEEP()
{
  eeprom_update_block(&iniSettings,&settingsEEP,sizeof(eepBlock_t));
  readEEP();
}
void EEPM::executeSleepSubscription()
{
  if(eep_state == CHANGED)
  {
    saveEEP();
  }
}
