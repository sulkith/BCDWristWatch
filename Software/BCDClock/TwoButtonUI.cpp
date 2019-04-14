#include "TwoButtonUI.h"
#include "ClockM.h"
#include "EEPM.h"
#include "SleepM.h"

const uint16_t ontime_short=5;
const uint16_t ontime_long=30;

void TwoButtonUI::stateTransition()
{
  if(inputEnabled == false && rightButton->getValue() == 0 && leftButton->getValue() == 0)
  {
    inputEnabled = true;
  }
  else
  {
    if(inputEnabled==false)return;
  }

  uint8_t wakeTime = ontime_short;
  switch (UIstate) {
    case Time:
      if(rightButton->valueUpdatedTo(1) && leftButton->getValue()==1)UIstate = SetHour;
      break;
      case SetHour:
        if(leftButton->valueUpdatedTo(1))ClockM::getInstance().advanceHour();
        if(rightButton->valueUpdatedTo(1))UIstate = SetMinute;
        wakeTime = ontime_long;
        break;
    case SetMinute:
      if(leftButton->valueUpdatedTo(1))ClockM::getInstance().advanceMinute();
      if(rightButton->valueUpdatedTo(1))UIstate = Time;
      wakeTime = ontime_long;
      break;

  }

  if(rightButton->getValue() == 1)SleepM::requestProlong(wakeTime);
  if(leftButton->getValue() == 1)SleepM::requestProlong(wakeTime);
  /*if(rightButton->getValue()==0)
    UIstate = Time;
  else
    UIstate = SetMinute;*/
}

inline void requestScreen(DisplayManager *dm, DisplayRequestType type, uint16_t d0, uint16_t d1=0, uint16_t d2=0, uint16_t d3=0)
{
  uint16_t data[DisplayRequest::dataLength] = {0};
  data[0] = d0;
  data[1] = d1;
  data[2] = d2;
  data[3] = d3;
  DisplayRequest dr(type,data);
  dm->requestDisplay(dr);
}

void TwoButtonUI::stateDisplayReuest()
{
  uint16_t data[DisplayRequest::dataLength] = {0};
  switch (UIstate) {
    case Time:
    case SetHour:
    case SetMinute:
      requestScreen(DisplMan,UIstate,ClockM::getInstance().getHour(),ClockM::getInstance().getMinute(),ClockM::getInstance().getSecond());
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
  inputEnabled = false;
}
