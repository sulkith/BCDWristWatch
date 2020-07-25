#ifndef __SLEEPM_HEADER_INCLUDE__
#define __SLEEPM_HEADER_INCLUDE__
#include <ClockM.hpp>
#include "HAL.hpp"
#include "SleepM.hpp"
#include "SleepSubscriber.hpp"

const uint8_t maxSubscribers = 5;
class SleepM
{
  HAL *hal;
  uint32_t wakeupTime = 0;
  uint32_t sleepTime = 5;
  static SleepM *instance;
  uint8_t numSubscriber = 0;
  SleepSubscriber *subscriber[maxSubscribers];
  SleepM(HAL *halptr):hal(halptr){}
  public:
  SleepM()
  {
	  wakeupTime = ClockM::getInstance().getCtr();
	  sleepTime = wakeupTime+5;
  };
  void cyclic();
  void subscribe(SleepSubscriber *ref)
  {
    if(numSubscriber<maxSubscribers)
    {
      subscriber[numSubscriber++]=ref;
    }
  }
  void setHal(HAL *h){hal=h;}
  static void requestProlong(uint8_t t){if(instance!=0)instance->prolong(t);}
  void prolong(uint8_t t);
  static SleepM *getInstance(){return instance;}
};
#endif //__SLEEPM_HEADER_INCLUDE__
