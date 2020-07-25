#ifndef __GFORCEUI_HEADER_INCLUDE__
#define __GFORCEUI_HEADER_INCLUDE__
#include "UserInterface.hpp"
#include "GForceHAL.hpp"
#include "HAL.hpp"
#include "DisplayManager.hpp"
#include "DisplayRequest.hpp"
#include "SleepSubscriber.hpp"

class GForceUI: public UserInterface, public SleepSubscriber
{
  HAL *mHal;
  DisplayManager *DisplMan;
  GForceHAL *gHAL;
  DisplayRequestType UIstate = FadeIn;
  bool inputEnabled = false;
  uint8_t debouncerGForce = 0;
  uint8_t HistCtr = 0;
  int16_t debounce = 0;
  void stateTransition();
  void stateDisplayReuest();
  virtual void init();
public:
  virtual void init(DisplayManager *dm, GForceHAL *gh, HAL *h);
  virtual void cyclic();
  GForceUI(): mHal(NULL), DisplMan(NULL),gHAL(NULL){};
  virtual void executeSleepSubscription();
};

#endif //__GFORCEUI_HEADER_INCLUDE__
