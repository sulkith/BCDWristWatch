#ifndef __BINARYGWATCH_HEADER_INCLUDE__
#define __BINARYGWATCH_HEADER_INCLUDE__

#include "HAL.h"
#include "GForceHAL.h"
#include "TwoButtonHAL.h"
#include "DisplayManager.h"

class BinaryGWatch: public HAL, public TwoButtonHAL, public GForceHAL, public DisplayManager
{
  uint8_t wakeupReason=1;//TODO ONLY TEST SHOULD BE 0
  uint16_t cyclicCounter=1000;//best case guess
  uint32_t stepsOffset = 0;
  uint16_t stepsHist[4] = {0};
  //for testing
  //uint16_t stepsHist[4] = {11111,22222,33333,44444};
  uint32_t stepsMeasured = 0;
  static const uint8_t UBattFilterLen = 5;
  uint8_t UBattCtr;
  uint16_t UBattFilterValues[UBattFilterLen];
  uint16_t UBatt;
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
  virtual void updateSteps();
  virtual void show();
  virtual void pushNewSteps(uint16_t steps);
  virtual uint16_t getHistSteps(uint8_t days);
  virtual void setupUBattMeasure();
  virtual void ADC_cyclic();
  virtual uint16_t getUBatt();
};

#endif //__BINARYGWATCH_HEADER_INCLUDE__
