#ifndef ___TWOBUTTONUI_HEADER_INCLUDE__
#define ___TWOBUTTONUI_HEADER_INCLUDE__
#include "UserInterface.h"
#include "TwoButtonHAL.h"
#include "HAL.h"
#include "Debouncer.h"
#include "DisplayManager.h"
#include "DisplayRequest.h"
#include "SleepSubscriber.h"

class TwoButtonUI: public UserInterface, public SleepSubscriber
{
  Debouncer<uint8_t> *rightButton;
  Debouncer<uint8_t> *leftButton;
  HAL *hal;
  DisplayManager *DisplMan;
  DisplayRequestType UIstate = Time;
  bool inputEnabled = false;
  void stateTransition();
  void stateDisplayReuest();
public:
  virtual void cyclic();
  virtual void init();
  TwoButtonUI(HAL *hwhal,TwoButtonHAL *tbh, DisplayManager *dm):
  rightButton(tbh->getRightButtonDeb()), leftButton(tbh->getLeftButtonDeb()),
  hal(hwhal), DisplMan(dm){};
  TwoButtonUI() = delete;
  virtual void executeSleepSubscription();
};

#endif //___TWOBUTTONUI_HEADER_INCLUDE__
