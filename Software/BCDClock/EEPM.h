#include <stdint.h>
#include "SleepSubsriber.h"

typedef enum{
  UNINITIALIZED,
  NORMAL,
  CHANGED
} EEPState_t;

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
  EEPState_t eep_state = UNINITIALIZED;
  static EEPM instance;
  EEPM(){}
public:

  void saveEEP();
  void readEEP();
  void iniEEP();
  uint8_t getCorrEveryMinute(){return eep.corrEveryMinute;}
  uint8_t getCorrEveryHour(){return eep.corrEveryHour;}
  uint8_t getCorrEveryDay(){return eep.corrEveryDay;}
  uint8_t getCorrEveryMonth(){return eep.corrEveryMonth;}
  virtual void executeSleepSubscription();
  static EEPM& getInstance()
  {
      return instance;
  }
  EEPM(EEPM const&)               = delete;
  void operator=(EEPM const&)     = delete;
};
