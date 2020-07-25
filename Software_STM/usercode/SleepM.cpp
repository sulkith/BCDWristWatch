#include "SleepM.hpp"

SleepM sm;
SleepM *SleepM::instance = &sm;

void SleepM::cyclic()
{
  if(ClockM::getInstance().getCtr() > sleepTime)
  {
    for(uint8_t i=0;i<numSubscriber;++i)
    {
      subscriber[i]->executeSleepSubscription();
    }
    if(hal!=0)hal->HAL_sleep();
   //It wont get here anyway
  }
}
void SleepM::prolong(uint8_t t){
  sleepTime = ClockM::getInstance().getCtr()+t;
}
