#ifndef __GFORCE_HEADER_INCLUDE__
#define __GFORCE_HEADER_INCLUDE__
#include <stdint.h>
#include "HAL.hpp"
class GForceHAL: public HAL
{
public:
  virtual uint8_t getTap()=0;
  virtual int16_t getX()=0;
  virtual int16_t getY()=0;
  virtual int16_t getZ()=0;
  virtual uint32_t getSteps()=0;
  virtual uint16_t getHistSteps(uint8_t days)=0;
  virtual uint8_t getHistStepsSize()=0;
  virtual uint16_t getUBatt()=0;
};
#endif //__GFORCE_HEADER_INCLUDE__
