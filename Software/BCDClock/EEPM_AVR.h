#ifndef __EEPMM_AVR_HEADER_INCLUDE__
#define __EEPMM_AVR_HEADER_INCLUDE__

#include <stdint.h>
#include "EEPM.h"

typedef enum{
  UNINITIALIZED,
  NORMAL,
  CHANGED
} EEPState_t;

class EEPM_AVR: public EEPM
{
  eepBlock_t eep;
  EEPState_t eep_state = UNINITIALIZED;
public:
  EEPM_AVR();
  virtual void saveEEP();
  virtual void readEEP();
  virtual void iniEEP();
  virtual void executeSleepSubscription();
  EEPM_AVR(EEPM_AVR const&)           = delete;
  void operator=(EEPM_AVR const&)     = delete;
  virtual uint8_t getCorrEveryMinute(){return eep.corrEveryMinute;}//Dummy Implementation
  virtual uint8_t getCorrEveryHour(){return eep.corrEveryHour;}//Dummy Implementation
  virtual uint8_t getCorrEveryDay(){return eep.corrEveryDay;}//Dummy Implementation
  virtual uint8_t getCorrEveryMonth(){return eep.corrEveryMonth;}//Dummy Implementation
  virtual uint16_t getActivatedFeatures(){return eep.activatedFeatures;}//Dummy Implementation
  virtual uint16_t getTempOffset(){return eep.tempOffset;}//Dummy Implementation
  virtual void setCorrEveryMinute(uint8_t t);
  virtual void setCorrEveryHour(uint8_t t);
  virtual void setCorrEveryDay(uint8_t t);
  virtual void setCorrEveryMonth(uint8_t t);
  virtual void setActivatedFeatures(uint16_t t);
  virtual void setTempOffset(uint16_t t);
};
#endif //__EEPMM_AVR_HEADER_INCLUDE__
