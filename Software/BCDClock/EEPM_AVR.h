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
};
#endif //__EEPMM_AVR_HEADER_INCLUDE__
