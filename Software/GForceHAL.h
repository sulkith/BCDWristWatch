#ifndef __GFORCE_HEADER_INCLUDE__
#define __GFORCE_HEADER_INCLUDE__
#include <stdint.h>

class GForceHAL
{
public:
  virtual uint8_t getTap();
  virtual int16_t getX();
  virtual int16_t getY();
  virtual int16_t getZ();
};
#endif //__GFORCE_HEADER_INCLUDE__
