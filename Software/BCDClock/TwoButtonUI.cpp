#include "TwoButtonUI.h"
#include "ClockM.h"
#include "EEPM.h"

void TwoButtonUI::stateTransition(){}
void TwoButtonUI::stateDisplayReuest()
{
  uint16_t data[DisplayRequest::dataLength] = {0};
  switch (UIstate) {
    case Time:
      data[0] = ClockM::getInstance().getHour();
      data[1] = ClockM::getInstance().getMinute();
      data[2] = ClockM::getInstance().getSecond();
      DisplayRequest dr(UIstate,data);
      DisplMan->requestDisplay(dr);
      break;
  }

}


void TwoButtonUI::cyclic()
{
  if(hal->HAL_getWakeupReason()>0)
  {
    stateTransition();
    stateDisplayReuest();
  }
}
void TwoButtonUI::executeSleepSubscription()
{
  UIstate = Time;
}
