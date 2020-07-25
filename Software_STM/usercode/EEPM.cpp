#include "settings.hpp"
#include "EEPM.hpp"

EEPM *EEPM::instance = 0;

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
/*
void EEPM::setCorrEveryMinute(uint8_t t)
{
  if(t<60)
  {
    //eep_state = CHANGED;
    eep.corrEveryMinute = t;
  }
}
void EEPM::setCorrEveryHour(uint8_t t)
{
  if(t<60)
  {
    //eep_state = CHANGED;
    eep.corrEveryHour = t;
  }
}
void EEPM::setCorrEveryDay(uint8_t t)
{
  if(t<24)
  {
    //eep_state = CHANGED;
    eep.corrEveryDay = t;
  }
}
void EEPM::setCorrEveryMonth(uint8_t t)
{
  if(t<30)
  {
    //eep_state = CHANGED;
    eep.corrEveryMonth = t;
  }
}
void EEPM::setActivatedFeatures(uint16_t t)
{
    //eep_state = CHANGED;
    eep.activatedFeatures = t;
}
void EEPM::setTempOffset(uint16_t t)
{
    //eep_state = CHANGED;
    eep.tempOffset = t;
}
*/
