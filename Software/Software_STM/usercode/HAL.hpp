#ifndef __HAL_HEADER_INCLUDE__
#define __HAL_HEADER_INCLUDE__
#include <stdint.h>
class HAL
{
public:
  virtual void HAL_init()=0;
  virtual void HAL_driverInit()=0;
  virtual uint8_t HAL_sleep()=0;
  virtual void HAL_lockInts()=0;
  virtual void HAL_releaseInts()=0;
  virtual uint8_t HAL_getWakeupReason()=0;
  virtual void HAL_cyclic()=0;
};
#endif //__HAL_HEADER_INCLUDE__
