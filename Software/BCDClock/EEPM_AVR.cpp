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
  EEPM::setInstance(this);
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

void EEPM_AVR::setCorrEveryMinute(uint8_t t)
{
  if(t<60)
  {
    eep_state = CHANGED;
    eep.corrEveryMinute = t;
  }
}
void EEPM_AVR::setCorrEveryHour(uint8_t t)
{
  if(t<60)
  {
    eep_state = CHANGED;
    eep.corrEveryHour = t;
  }
}
void EEPM_AVR::setCorrEveryDay(uint8_t t)
{
  if(t<24)
  {
    eep_state = CHANGED;
    eep.corrEveryDay = t;
  }
}
void EEPM_AVR::setCorrEveryMonth(uint8_t t)
{
  if(t<30)
  {
    eep_state = CHANGED;
    eep.corrEveryMonth = t;
  }
}
void EEPM_AVR::setActivatedFeatures(uint16_t t)
{
    eep_state = CHANGED;
    eep.activatedFeatures = t;
}
void EEPM_AVR::setTempOffset(uint16_t t)
{
    eep_state = CHANGED;
    eep.tempOffset = t;
}
