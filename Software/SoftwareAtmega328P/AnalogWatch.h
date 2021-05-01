#ifndef __ANALOGWATCH_HEADER_INCLUDE__
#define __ANALOGWATCH_HEADER_INCLUDE__

#include "HAL.h"
#include "TwoButtonHAL.h"
#include "DisplayManager.h"

class AnalogWatch: public HAL, public TwoButtonHAL, public DisplayManager
{
  uint8_t wakeupReason=1;//TODO ONLY TEST SHOULD BE 0
  uint16_t cyclicCounter=1000;//best case guess
public:
  virtual void HAL_init();
  virtual uint8_t HAL_sleep();
  virtual void HAL_lockInts();
  virtual void HAL_releaseInts();
  virtual uint8_t HAL_getWakeupReason();
  virtual void HAL_cyclic();
  virtual Debouncer<uint8_t> *getRightButtonDeb();
  virtual Debouncer<uint8_t> *getLeftButtonDeb();
  virtual void show();
};

#endif //__ANALOGWATCH_HEADER_INCLUDE__
