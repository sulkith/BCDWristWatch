#ifndef __EEPMM_HEADER_INCLUDE__
#define __EEPMM_HEADER_INCLUDE__

#include <stdint.h>
#include "SleepSubscriber.h"

typedef struct{
  uint16_t eepVers;//identifies the Version of the EEP
  uint8_t corrEveryMinute;
	uint8_t corrEveryHour;
	uint8_t corrEveryDay;
	uint8_t corrEveryMonth;
  uint16_t activatedFeatures;
  uint16_t tempOffset;
} eepBlock_t;

class EEPM: public SleepSubscriber
{
  eepBlock_t eep;
  static EEPM instance;
public:
  EEPM();
  virtual void saveEEP(){};//Dummy Implementation
  virtual void readEEP(){};//Dummy Implementation
  virtual void iniEEP(){};//Dummy Implementation
  uint8_t getCorrEveryMinute(){return eep.corrEveryMinute;}//Dummy Implementation
  uint8_t getCorrEveryHour(){return eep.corrEveryHour;}//Dummy Implementation
  uint8_t getCorrEveryDay(){return eep.corrEveryDay;}//Dummy Implementation
  uint8_t getCorrEveryMonth(){return eep.corrEveryMonth;}//Dummy Implementation
  virtual void executeSleepSubscription(){saveEEP();};//Dummy Implementation
  EEPM(EEPM const&)               = delete;
  void operator=(EEPM const&)     = delete;
};
#endif //__EEPMM_HEADER_INCLUDE__
