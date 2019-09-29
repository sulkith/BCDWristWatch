#ifndef __GFORCEUI_HEADER_INCLUDE__
#define __GFORCEUI_HEADER_INCLUDE__
#include "UserInterface.h"
#include "GForceHAL.h"
#include "TwoButtonHAL.h"
#include "HAL.h"
#include "Debouncer.h"
#include "DisplayManager.h"
#include "DisplayRequest.h"
#include "SleepSubscriber.h"

class GForceUI: public UserInterface, public SleepSubscriber
{
  Debouncer<uint8_t> *rightButton;
  Debouncer<uint8_t> *leftButton;
  HAL *mHal;
  DisplayManager *DisplMan;
  GForceHAL *gHAL;
  DisplayRequestType UIstate = FadeIn;
  bool inputEnabled = false;
  uint8_t debouncerGForce = 0;
  int16_t debounce = 0;
  void stateTransition();
  void stateDisplayReuest();
public:
  virtual void cyclic();
  virtual void init();
  GForceUI():rightButton(NULL), leftButton(NULL),
  mHal(NULL), DisplMan(NULL),gHAL(NULL){};
  virtual void executeSleepSubscription();
};

#endif //__GFORCEUI_HEADER_INCLUDE__
