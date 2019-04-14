#ifndef __TWOBUTTONHAL_HEADER_INCLUDE__
#define __TWOBUTTONHAL_HEADER_INCLUDE__
#include "Debouncer.h"

const uint16_t Button_debounceTime=30;

class TwoButtonHAL
{
public:
  virtual Debouncer<uint8_t> *getRightButtonDeb()=0;
  virtual Debouncer<uint8_t> *getLeftButtonDeb()=0;
  //virtual uint8_t getLeftButton()=0;
  //virtual uint8_t getRightButton()=0;
};
#endif //__TWOBUTTONHAL_HEADER_INCLUDE__
