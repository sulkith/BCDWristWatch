#include "SleepM.h"

SleepM *SleepM::instance = 0;

void SleepM::cyclic()
{
  if(ClockM::getInstance().getCtr() > sleepTime)
  {
    for(uint8_t i=0;i<numSubscriber;++i)
    {
      subscriber[i]->executeSleepSubscription();
    }
    while(hal->HAL_sleep()==0);
    wakeupTime = ClockM::getInstance().getCtr();
    sleepTime = wakeupTime+2;
  }
}
void SleepM::prolong(uint8_t t){
  sleepTime = ClockM::getInstance().getCtr()+t;
}
