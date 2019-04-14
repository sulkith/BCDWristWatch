#ifndef __BINARYWATCH_HEADER_INCLUDE__
#define __BINARYWATCH_HEADER_INCLUDE__

#include "HAL.h"
#include "TwoButtonHAL.h"
#include "DisplayManager.h"

class BinaryWatch: public HAL, public TwoButtonHAL, public DisplayManager
{
  uint8_t wakeupReason=1;//TODO ONLY TEST SHOULD BE 0
public:
  virtual void HAL_init();
  virtual uint8_t HAL_sleep();
  virtual void HAL_lockInts();
  virtual void HAL_releaseInts();
  virtual uint8_t HAL_getWakeupReason();
  virtual Debouncer<uint8_t> *getRightButtonDeb();
  virtual Debouncer<uint8_t> *getLeftButtonDeb();
  virtual void show();
};

#endif //__BINARYWATCH_HEADER_INCLUDE__
