#include "SleepM.h"

void SleepM::cyclic()
{
  if(ClockM::getInstance().getCtr() > sleepTime)
  {
    while(hal->HAL_sleep()==0);
    wakeupTime = ClockM::getInstance().getCtr();
    sleepTime = wakeupTime+5;
  }
}
