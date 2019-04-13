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
  ClockM(){}
  static ClockM instance;
public:
  uint8_t operator++();
  uint8_t getSecond(){return t.second;}
  uint8_t getMinute(){return t.minute;}
  uint8_t getHour(){return t.hour;}
  uint8_t advanceMinute();
  uint8_t advanceHour();

  static ClockM& getInstance()
  {
      return instance;
  }
  ClockM(ClockM const&)             = delete;
  void operator=(ClockM const&)     = delete;

};
#endif //__CLOCKM_HEADER_INCLUDE__
