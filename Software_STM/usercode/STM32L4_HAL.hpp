#include "GForceHAL.hpp"
#include "DisplayManager.hpp"
#include "types.hpp"


const uint8_t WAKEUP_AlarmA = 0x01;
//Everything > 0x10 is interactive
const uint8_t WAKEUP_BMA_TAP = 0x12;
const uint8_t WAKEUP_BMA_TILT = 0x13;
const uint8_t WAKEUP_UNKNOWN = 0xEE;
const uint8_t WAKEUP_POR = 0xFF;
const uint8_t stepsHistSize = 4;

class STM32L4_HAL: public GForceHAL
{
	DisplayManager *dman_loc = NULL;
	Watch_Type_t AxisMappingVariant = Binary_v1;

	  uint8_t wakeupReason=WAKEUP_UNKNOWN;
	  uint16_t cyclicCounter=1000;//best case guess
	  uint32_t stepsOffset = 0;
	  uint32_t stepsMeasured = 0;
	  uint16_t stepsHist[stepsHistSize] = {0};
	  uint16_t UBatt;

	void showERROR(uint8_t error, uint8_t data);
	void setupBMA();
	void writeDataToBackupRegisters();
public:
  virtual void HAL_init();
  virtual uint8_t HAL_sleep();
  virtual void HAL_lockInts();
  virtual void HAL_releaseInts();
  virtual uint8_t HAL_getWakeupReason();
  virtual void HAL_cyclic();
  virtual uint8_t getTap();
  virtual int16_t getX();
  virtual int16_t getY();
  virtual int16_t getZ();
  virtual uint32_t getSteps();
  virtual void resetSteps();
  virtual uint16_t getHistSteps(uint8_t days);
  virtual uint16_t getUBatt();
  void setDisplayManager(DisplayManager *dman_arg);
  void setAxisMappingVariant(Watch_Type_t var);
  void updateSteps();
  void pushNewSteps(uint16_t steps);
};
