#ifndef __CLOCKM_HEADER_INCLUDE__
#define __CLOCKM_HEADER_INCLUDE__

#include "main.h"

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint16_t steps;
}  time_t;
class ClockM
{
  time_t t;
	uint32_t ctr;
	bool hour_changed = 0;
  static ClockM instance;
	ClockM();
	void updateLocalVars();
public:
  uint8_t getSecond(){updateLocalVars();return t.second;}
  uint8_t getMinute(){updateLocalVars();return t.minute;}
  uint8_t getHour(){updateLocalVars();return t.hour;}
	bool isHourChanged(){return hour_changed;}
	void setHourChanged(bool changed){hour_changed = changed;}
	uint32_t getCtr(){return HAL_GetTick()/1000;}
  uint8_t advanceMinute();
  uint8_t advanceHour();
uint8_t decreaseMinute();
  uint8_t decreaseHour();
  void clearSeconds();

  static ClockM& getInstance()
  {
      return instance;
  }
  ClockM(ClockM const&)             = delete;
  void operator=(ClockM const&)     = delete;

};
#endif //__CLOCKM_HEADER_INCLUDE__
