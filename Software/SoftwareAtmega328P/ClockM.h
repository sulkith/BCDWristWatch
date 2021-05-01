#ifndef __CLOCKM_HEADER_INCLUDE__
#define __CLOCKM_HEADER_INCLUDE__

#include <stdint.h> //stdint makes problems here because of missing include guards....
#include "EEPM.h"

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
	EEPM *eepm_ptr;
	bool hour_changed = 0;
	ClockM(uint8_t h, uint8_t m, uint8_t s);
  static ClockM instance;
public:
	void setEEPM(EEPM *ptr);
  uint8_t operator++();
  uint8_t getSecond(){return t.second;}
  uint8_t getMinute(){return t.minute;}
  uint8_t getHour(){return t.hour;}
	bool isHourChanged(){return hour_changed;}
	uint32_t getCtr(){return ctr;}
  uint8_t advanceMinute();
  uint8_t advanceHour();
	uint8_t decreaseMinute();
  uint8_t decreaseHour();

  static ClockM& getInstance()
  {
      return instance;
  }
  ClockM(ClockM const&)             = delete;
  void operator=(ClockM const&)     = delete;

};
#endif //__CLOCKM_HEADER_INCLUDE__
