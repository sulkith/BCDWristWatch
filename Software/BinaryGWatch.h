#ifndef __BINARYWATCH_HEADER_INCLUDE__
#define __BINARYWATCH_HEADER_INCLUDE__

#include "HAL.h"
#include "GForceHAL.h"
#include "TwoButtonHAL.h"
#include "DisplayManager.h"

class BinaryGWatch: public HAL, public TwoButtonHAL, public GForceHAL, public DisplayManager
{
  uint8_t wakeupReason=1;//TODO ONLY TEST SHOULD BE 0
  uint16_t cyclicCounter=1000;//best case guess
  uint32_t stepsOffset = 0;
public:
  virtual void HAL_init();
  virtual uint8_t HAL_sleep();
  virtual void HAL_lockInts();
  virtual void HAL_releaseInts();
  virtual uint8_t HAL_getWakeupReason();
  virtual void HAL_cyclic();
  virtual Debouncer<uint8_t> *getRightButtonDeb();
  virtual Debouncer<uint8_t> *getLeftButtonDeb();
  virtual uint8_t getTap();
  virtual int16_t getX();
  virtual int16_t getY();
  virtual int16_t getZ();
  virtual uint32_t getSteps();
  virtual void show();
};

#endif //__BINARYWATCH_HEADER_INCLUDE__
