#include "GForceUI.hpp"
#include "ClockM.hpp"
#include "SleepM.hpp"
#include "settings.hpp"

const uint16_t ontime_short = 4;
const uint16_t ontime_long = 30;
const uint16_t ontime_very_long = 60;

extern HAL *hal;
extern DisplayManager *dman;
extern GForceHAL *_gHAL;
extern UserInterface *UI;

//GForceUI tbui;
//UserInterface *UI = &tbui;

template<typename T>
uint8_t caseForAdjusting(T *const value, GForceHAL *gfh, const T maxvalue,
		const T minvalue = 0, const int16_t debounceThreshold = 50) {
	static int16_t debounce = 0;
	if (gfh->getTap()) {
		debounce = 0;
		return 1;
	}
	int16_t axis_val = gfh->getY();
	if (axis_val > 1024) {
		debounce++;
	} else if (axis_val < -1024) {
		debounce--;
	} else {
		debounce = 0;
	}
	if (debounce > debounceThreshold) {
		if ((*value) == maxvalue)
			(*value) = minvalue;
		else
			(*value)++;
		debounce = 0;
	}
	if (debounce < -debounceThreshold) {
		if ((*value) == minvalue)
			(*value) = maxvalue;
		else
			(*value)--;
		debounce = 0;
	}
	return 0;
}

void GForceUI::stateTransition() {
	if (UIstate == FadeIn) {
		UIstate = Time;
		SleepM::requestProlong(ontime_short);
	}

	switch (UIstate) {
	case FadeIn:
		UIstate = Time;
		break;
	case Debouncing:
	case Time_s:
	case Time:
		//debouncerGForce=1;//TODO Only for testing
		//UIstate = Debouncing;
		if (gHAL->getTap()) {
			if (debouncerGForce == 0) {
				if (gHAL->getZ() > 7000)			//Check if it is upside Down
						{
					debouncerGForce = 1;
					UIstate = Debouncing;
					SleepM::requestProlong(ontime_short);
				} else {
					if (gHAL->getY() > 300 && WatchSettingsPtr->enableDate) {
						UIstate = Date;
						SleepM::requestProlong(ontime_short);
						debouncerGForce = 0;
					} else if (gHAL->getY() < -300) {
						UIstate = showStepCounter;
						SleepM::requestProlong(ontime_short);
					} else {
#ifdef TIME_S
						if(WatchSettingsPtr->enableSeconds)
						{
							if(UIstate == Time_s)
							{
								UIstate = Time;
								SleepM::requestProlong(ontime_short);
							}
							else
							{
								UIstate = Time_s;
								SleepM::requestProlong(ontime_long);
							}
						}
						else
#endif
						{
							UIstate = showStepCounter;
							SleepM::requestProlong(ontime_short);
						}
					}
				}
			} else if (debouncerGForce == 1) {
				if (gHAL->getZ() < -7000)	//Check if it is in normal Position
				{
					UIstate = SetHour;
					SleepM::requestProlong(ontime_very_long);
					debouncerGForce = 0xFF;
				} else {
					UIstate = Time;
					SleepM::requestProlong(ontime_short);
					debouncerGForce = 0;
				}
			} else if (debouncerGForce == 0xFF) {
				debouncerGForce = 0;
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
			//SleepM::requestProlong(ontime_long);
		}
		break;
	case SetHour:
		if (gHAL->getTap()) {
			UIstate = SetMinute;
			SleepM::requestProlong(ontime_very_long);
		}
		if (debouncerGForce == 0xFF) {
			int16_t axis_val = gHAL->getY();
			if (axis_val > 1024) {
				debounce++;
			} else if (axis_val < -1024) {
				debounce--;
			} else {
				debounce = 0;
			}
			if (debounce > WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().advanceHour();
				debounce = 0;
			}
			if (debounce < -WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().decreaseHour();
				debounce = 0;
			}
		}
		break;
	case SetMinute:
		if (gHAL->getTap()) {
			UIstate = Time;
			ClockM::getInstance().clearSeconds();
			ClockM::getInstance().saveTime();
			SleepM::requestProlong(ontime_short);
		}
		if (debouncerGForce == 0xFF) {
			int16_t axis_val = gHAL->getY();
			if (axis_val > 1024) {
				debounce++;
			} else if (axis_val < -1024) {
				debounce--;
			} else {
				debounce = 0;
			}
			if (debounce > WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().advanceMinute();
				debounce = 0;
			}
			if (debounce < -WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().decreaseMinute();
				debounce = 0;
			}
		}
		break;
	case DebouncingDate:
	case Date:
		if (gHAL->getTap()) {
			if (debouncerGForce == 0) {
				if (gHAL->getZ() > 7000)			//Check if it is upside Down
						{
					debouncerGForce = 1;
					UIstate = DebouncingDate;
					SleepM::requestProlong(ontime_short);
				} else {
					UIstate = Time;
					SleepM::requestProlong(ontime_short);
				}
			} else if (debouncerGForce == 1) {
				if (gHAL->getZ() < -7000)	//Check if it is in normal Position
						{
					UIstate = SetDay;
					SleepM::requestProlong(ontime_very_long);
					debouncerGForce = 0xFF;
				} else {
					UIstate = Date;
					SleepM::requestProlong(ontime_short);
					debouncerGForce = 0;
				}
			} else if (debouncerGForce == 0xFF) {
				debouncerGForce = 0;
			}
		}
		break;
	case SetDay:
		if (gHAL->getTap()) {
			UIstate = SetMonth;
			SleepM::requestProlong(ontime_very_long);
		}
		if (debouncerGForce == 0xFF) {
			int16_t axis_val = gHAL->getY();
			if (axis_val > 1024) {
				debounce++;
			} else if (axis_val < -1024) {
				debounce--;
			} else {
				debounce = 0;
			}
			if (debounce > WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().advanceDay();
				debounce = 0;
			}
			if (debounce < -WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().decreaseDay();
				debounce = 0;
			}
		}
		break;
	case SetMonth:
		if (gHAL->getTap()) {
			UIstate = SetYear;
			SleepM::requestProlong(ontime_very_long);
		}
		if (debouncerGForce == 0xFF) {
			int16_t axis_val = gHAL->getY();
			if (axis_val > 1024) {
				debounce++;
			} else if (axis_val < -1024) {
				debounce--;
			} else {
				debounce = 0;
			}
			if (debounce > WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().advanceMonth();
				debounce = 0;
			}
			if (debounce < -WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().decreaseMonth();
				debounce = 0;
			}
		}
		break;
	case SetYear:
		if (gHAL->getTap()) {
			UIstate = Date;
			ClockM::getInstance().saveTime();
			SleepM::requestProlong(ontime_short);
		}
		if (debouncerGForce == 0xFF) {
			int16_t axis_val = gHAL->getY();
			if (axis_val > 1024) {
				debounce++;
			} else if (axis_val < -1024) {
				debounce--;
			} else {
				debounce = 0;
			}
			if (debounce > WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().advanceYear();
				debounce = 0;
			}
			if (debounce < -WatchSettingsPtr->debounceSetTime) {
				ClockM::getInstance().decreaseYear();
				debounce = 0;
			}
		}
		break;
	case showStepCounter:
		if (gHAL->getTap()) {
			if (gHAL->getZ() > 7000 && WatchSettingsPtr->enableStepHistory)//Check if it is upside Down
			{
				UIstate = ShowStepsHistory;
				SleepM::requestProlong(ontime_very_long);
			} else {
				UIstate = Time;
				SleepM::requestProlong(ontime_short);
			}
		}
		break;
	case ShowStepsHistory: {
		if (caseForAdjusting<uint8_t>(&HistCtr, gHAL, gHAL->getHistStepsSize()-1,0,WatchSettingsPtr->debounceStepsHistory)) {
			UIstate = ShowUBatt;
			SleepM::requestProlong(ontime_short);
		}
		break;
	}
	case ShowUBatt: {
		if (gHAL->getTap()) {
			UIstate = Time;
			SleepM::requestProlong(ontime_short);
		}
		break;
	}
		break;
	case showError:
	case Raw_Output:
	case Empty:
		UIstate = Time;
	}

}

inline void requestScreen(DisplayManager *dm, DisplayRequestType type,
		uint16_t d0, uint16_t d1 = 0, uint16_t d2 = 0, uint16_t d3 = 0,
		uint16_t d4 = 0) {
	uint16_t data[DisplayRequest::dataLength] = { 0 };
	data[0] = d0;
	data[1] = d1;
	data[2] = d2;
	data[3] = d3;
	data[4] = d4;
	DisplayRequest dr(type, data);
	dm->requestDisplay(dr);
}

void GForceUI::init() {
}

void GForceUI::stateDisplayReuest() {
	//uint16_t data[DisplayRequest::dataLength] = {0};
	switch (UIstate) {
	case FadeIn:
	case Time:
	case Time_s:
	case SetHour:
	case SetMinute:
	case Debouncing:
		requestScreen(DisplMan, UIstate, ClockM::getInstance().getHour(),
				ClockM::getInstance().getMinute(),
				ClockM::getInstance().getSecond());
		break;
	case Date:
	case SetDay:
	case SetMonth:
	case SetYear:
	case DebouncingDate:
		requestScreen(DisplMan, UIstate, ClockM::getInstance().getDay(),
				ClockM::getInstance().getMonth(),
				ClockM::getInstance().getYear());
		break;
	case showStepCounter: {
		uint32_t steps = gHAL->getSteps();
		uint32_t steps_most_significant = steps;
		uint8_t exp = 0;
		for (; steps_most_significant > 99;
				steps_most_significant = steps_most_significant / 10) {
			exp++;
		}
		requestScreen(DisplMan, UIstate, steps_most_significant, exp, steps);
		break;
	}
	case ShowStepsHistory: {
		requestScreen(DisplMan, UIstate, HistCtr,0, gHAL->getHistSteps(HistCtr));
//				gHAL->getHistSteps(1), gHAL->getHistSteps(2),
//				gHAL->getHistSteps(3));
		break;
	}
	case ShowUBatt: {
		requestScreen(DisplMan, UIstate, gHAL->getUBatt());
		break;
	}
	case Empty:
	default:
		requestScreen(DisplMan, Empty, 0);
		break;
	}

}

void GForceUI::cyclic() {
	if (mHal->HAL_getWakeupReason() > 0) {
		stateDisplayReuest();
		switch (UIstate) {
		case SetDay:
		case SetHour:
		case SetMinute:
		case SetMonth:
		case SetYear:
			break;
		default:
			ClockM::getInstance().updateTime();
			break;
		}
		stateTransition();
	}
}

void GForceUI::init(DisplayManager *dm, GForceHAL *gh, HAL *h) {
	DisplMan = dm;
	gHAL = gh;
	mHal = h;
	//SleepM::getInstance()->subscribe(this);//Not needed since we will do a reset
}

void GForceUI::executeSleepSubscription() {
//	UIstate = FadeIn;
//	inputEnabled = false;
//	debouncerGForce = 0;
}

