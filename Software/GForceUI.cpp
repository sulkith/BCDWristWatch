#include "GForceUI.h"
#include "ClockM.h"
#include "EEPM.h"
#include "SleepM.h"

const uint16_t ontime_short=5;
const uint16_t ontime_long=30;
const uint16_t ontime_very_long=60;

extern HAL *hal;
extern DisplayManager *dman;
extern TwoButtonHAL *tbh;
extern GForceHAL *_gHAL;
extern UserInterface *UI;

GForceUI tbui;
UserInterface *UI = &tbui;

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

void GForceUI::stateTransition()
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
		case Debouncing:
    case Time:
			//debouncerGForce=1;//TODO Only for testing
			//UIstate = Debouncing;
      if(rightButton->valueUpdatedTo(1) && leftButton->getValue()==1)UIstate = SetHour;
			if(gHAL->getTap())
			{
				if(debouncerGForce == 0)
				{
					if(gHAL->getZ() < -7000)//Check if it is upside Down
					{
						debouncerGForce = 1;
						UIstate = Debouncing;
					}
				}
				else if(debouncerGForce == 1)
				{
					if(gHAL->getZ() > 7000)//Check if it is in normal Position
					{
						//debouncerGForce = 2;//To hard to achieve
						UIstate = SetHour;
						SleepM::requestProlong(ontime_very_long);
						debouncerGForce = 0xFF;
					}
					else
					{
						UIstate = Time;
						debouncerGForce = 0;
					}
				}
				/*if(0&&debouncerGForce == 2)//To hard to achieve
				{
					if(gHAL->getZ() > 7000)//Check if it is normal Position
					{
						UIstate = SetHour;
						SleepM::requestProlong(ontime_very_long);
					}
					debouncerGForce = 0;
				}*/
				SleepM::requestProlong(ontime_long);
			}
      wakeTime = ontime_short;
      break;
    case SetHour:
      if(LEFT_PUSH)ClockM::getInstance().advanceHour();
      if(RIGHT_PUSH)UIstate = SetMinute;
			if(gHAL->getTap())
			{
				UIstate = SetMinute;
				SleepM::requestProlong(ontime_very_long);
			}
			if(debouncerGForce == 0xFF)
			{
				int16_t axis_val = -gHAL->getY();
				if(axis_val > 1024)
				{
					debounce++;
				}
				else if(axis_val < -1024)
				{
					debounce--;
				}
				else
				{
					debounce = 0;
				}
				if(debounce > 50)
				{
					ClockM::getInstance().advanceHour();
					debounce = 0;
				}
				if(debounce < -50)
				{
					ClockM::getInstance().decreaseHour();
					debounce = 0;
				}
			}
      break;
    case SetMinute:
      if(LEFT_PUSH)ClockM::getInstance().advanceMinute();
      if(RIGHT_PUSH)UIstate = Time;
      if(LEFT_HOLD && RIGHT_HOLD) UIstate = SetCorrMinute;
			if(gHAL->getTap())
			{
				if(gHAL->getZ() < -7000)//Check if it is upside Down maybe check the Status from Hour also to make sure it is intended)
				{
					UIstate = SetCorrMinute;
				}
				else
				{
					UIstate = Time;
				}
			}
			if(debouncerGForce == 0xFF)
			{
				int16_t axis_val = -gHAL->getY();
				if(axis_val > 1024)
				{
					debounce++;
				}
				else if(axis_val < -1024)
				{
					debounce--;
				}
				else
				{
					debounce = 0;
				}
				if(debounce > 50)
				{
					ClockM::getInstance().advanceMinute();
					debounce = 0;
				}
				if(debounce < -50)
				{
					ClockM::getInstance().decreaseMinute();
					debounce = 0;
				}
			}
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

void GForceUI::init()
{
	SleepM::getInstance()->subscribe(this);

	if(rightButton == NULL)rightButton = tbh->getRightButtonDeb();
	if(leftButton == NULL) leftButton = tbh->getLeftButtonDeb();
	if(mHal==NULL) mHal = hal;
	if(DisplMan == NULL) DisplMan = dman;
	if(gHAL == NULL) gHAL = _gHAL;
}

void GForceUI::stateDisplayReuest()
{
  //uint16_t data[DisplayRequest::dataLength] = {0};
  switch (UIstate) {
		case FadeIn:
    case Time:
    case SetHour:
    case SetMinute:
		case Debouncing:
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


void GForceUI::cyclic()
{
  if(mHal->HAL_getWakeupReason()>0)
  {
		stateDisplayReuest();
		stateTransition();
  }
}
void GForceUI::executeSleepSubscription()
{
  UIstate = FadeIn;
  inputEnabled = false;
	debouncerGForce = 0;
}
