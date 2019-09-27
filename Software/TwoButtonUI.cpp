#include "TwoButtonUI.h"
#include "ClockM.h"
#include "EEPM.h"
#include "SleepM.h"

const uint16_t ontime_short=5;
const uint16_t ontime_long=30;

#define RIGHT_PUSH rightButton->valueUpdatedTo(1)
#define LEFT_PUSH leftButton->valueUpdatedTo(1)
#define RIGHT_HOLD rightButton->getValue()
#define LEFT_HOLD leftButton->getValue()
#define PUSH_BOTH (RIGHT_PUSH && LEFT_HOLD) || (LEFT_PUSH && RIGHT_HOLD)

template <typename T>
uint8_t caseForAdjusting(T* const value, Debouncer<uint8_t> *rightButton, Debouncer<uint8_t> *leftButton, const T maxvalue, const T minvalue=0)
{
	if(rightButton->valueUpdatedTo(1)==1)
	{
		if((*value)==minvalue)
			(*value)=maxvalue;
		else
			(*value)--;
		if(leftButton->getValue() == 1 && rightButton->getValue() == 1)
		{
			return 1;
		}
	}
	if(leftButton->valueUpdatedTo(1)==1)
	{
		if((*value)==maxvalue)
			(*value)=minvalue;
		else
			(*value)++;
		if(leftButton->getValue() == 1 && rightButton->getValue() == 1)
		{
			return 1;
		}
	}
	return 0;
}

void TwoButtonUI::stateTransition()
{
	if(UIstate == FadeIn)
	{
		UIstate = Time;
		SleepM::requestProlong(ontime_short);
	}
  if(inputEnabled == false && rightButton->getValue() == 0 && leftButton->getValue() == 0)
  {
    inputEnabled = true;
  }
  else
  {
    if(inputEnabled==false)return;
  }

  uint8_t wakeTime = ontime_long;
  switch (UIstate) {
		case FadeIn:
			UIstate =  Time;
			wakeTime = ontime_short;
			break;
    case Time:
      if(rightButton->valueUpdatedTo(1) && leftButton->getValue()==1)UIstate = SetHour;
      wakeTime = ontime_short;
      break;
    case SetHour:
      if(LEFT_PUSH)ClockM::getInstance().advanceHour();
      if(RIGHT_PUSH)UIstate = SetMinute;
      break;
    case SetMinute:
      if(LEFT_PUSH)ClockM::getInstance().advanceMinute();
      if(RIGHT_PUSH)UIstate = Time;
      if(LEFT_HOLD && RIGHT_HOLD) UIstate = SetCorrMinute;
      break;
    case SetCorrMinute:
    {
      uint8_t t = EEPM::getInstance()->getCorrEveryMinute();
      if(caseForAdjusting<uint8_t>(&t,rightButton,leftButton,30))UIstate = SetCorrHour;
      EEPM::getInstance()->setCorrEveryMinute(t);
      break;
    }
    case SetCorrHour:
    {
      uint8_t t = EEPM::getInstance()->getCorrEveryHour();
      if(caseForAdjusting<uint8_t>(&t,rightButton,leftButton,30))UIstate = SetCorrDay;
      EEPM::getInstance()->setCorrEveryHour(t);
      break;
    }
    case SetCorrDay:
    {
      uint8_t t = EEPM::getInstance()->getCorrEveryDay();
      if(caseForAdjusting<uint8_t>(&t,rightButton,leftButton,24))UIstate = SetCorrMonth;
      EEPM::getInstance()->setCorrEveryDay(t);
      break;
    }
    case SetCorrMonth:
    {
      uint8_t t = EEPM::getInstance()->getCorrEveryMonth();
      if(caseForAdjusting<uint8_t>(&t,rightButton,leftButton,30))UIstate = Time;
      EEPM::getInstance()->setCorrEveryMonth(t);
      break;
    }
    case Empty:
    case SetTempCorr:
    case ShowTemperature:
      UIstate = Time;
  }

  if(rightButton->getValue() == 1)SleepM::requestProlong(wakeTime);
  if(leftButton->getValue() == 1)SleepM::requestProlong(wakeTime);
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

void TwoButtonUI::init()
{
	SleepM::getInstance()->subscribe(this);
}

void TwoButtonUI::stateDisplayReuest()
{
  //uint16_t data[DisplayRequest::dataLength] = {0};
  switch (UIstate) {
		case FadeIn:
    case Time:
    case SetHour:
    case SetMinute:
      requestScreen(DisplMan,UIstate,ClockM::getInstance().getHour(),ClockM::getInstance().getMinute(),ClockM::getInstance().getSecond());
      break;
    case SetCorrMinute:
      requestScreen(DisplMan, UIstate, EEPM::getInstance()->getCorrEveryMinute());
      break;
    case SetCorrHour:
      requestScreen(DisplMan, UIstate, EEPM::getInstance()->getCorrEveryHour());
      break;
    case SetCorrDay:
      requestScreen(DisplMan, UIstate, EEPM::getInstance()->getCorrEveryDay());
      break;
    case SetCorrMonth:
      requestScreen(DisplMan, UIstate, EEPM::getInstance()->getCorrEveryMonth());
      break;
    case SetTempCorr:
      requestScreen(DisplMan, UIstate, EEPM::getInstance()->getTempOffset());
      break;
    case ShowTemperature:
    case Empty:
    default:
      requestScreen(DisplMan, Empty, 0);
      break;
  }

}


void TwoButtonUI::cyclic()
{
  if(hal->HAL_getWakeupReason()>0)
  {
		stateDisplayReuest();
		stateTransition();
  }
}
void TwoButtonUI::executeSleepSubscription()
{
  UIstate = FadeIn;
  inputEnabled = false;
}
