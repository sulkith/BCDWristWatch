#ifndef __SLEEPM_HEADER_INCLUDE__
#define __SLEEPM_HEADER_INCLUDE__
#include "ClockM.h"
#include "HAL.h"
#include "SleepM.h"
#include "SleepSubscriber.h"

class SleepM
{
  HAL *hal;
  uint32_t wakeupTime = 0;
  uint32_t sleepTime = 5;
  public:
  SleepM(HAL *halptr):hal(halptr){}
  void cyclic();
};
#endif //__SLEEPM_HEADER_INCLUDE__
