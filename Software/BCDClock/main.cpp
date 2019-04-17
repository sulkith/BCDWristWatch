#include "EEPM.h"
#include "ClockM.h"
#include "DisplayManager.h"
#include "DisplayRequest.h"
#include "HAL.h"
#include "SleepM.h"

#include "BinaryWatch.h"
#include "EEPM_AVR.h"
#include "TwoButtonUI.h"

extern HAL *hal;
extern DisplayManager *dman;
extern TwoButtonHAL *tbh;
extern UserInterface *UI;

void init();

int main(void)
{
  EEPM_AVR eep;
  EEPM::setInstance(&eep);
  eep.setCorrEveryHour(3);

  SleepM sleepM(hal);

  sleepM.subscribe(&eep);

  hal->HAL_init();
  ClockM::getInstance().setEEPM(&eep);

  if(0)
  {
    uint16_t data[4] = {0};
    uint16_t data2[4] = {0};
    data[0]=12;
    data[1]=48;
    DisplayRequest drsethr(SetHour, data);
    DisplayRequest drsetmn(SetMinute, data);
    DisplayRequest drsetmc(SetCorrMinute, data);
    DisplayRequest drsethc(SetCorrHour, data);
    DisplayRequest drsetdc(SetCorrDay, data);
    DisplayRequest drsetmoc(SetCorrMonth, data);
    data2[0] = 252;
    DisplayRequest drsettc(SetTempCorr, data2);
    DisplayRequest drst(ShowTemperature, data2);
    while(1)
    {
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drsethr);
        dman->show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drsetmn);
        dman->show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drsetmc);
        dman->show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drsethc);
        dman->show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drsetdc);
        dman->show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drsetmoc);
        dman->show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drsettc);
        dman->show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        dman->requestDisplay(drst);
        dman->show();
      }
    }
  }

  while(1)
  {
    hal->HAL_cyclic();
    UI->cyclic();
    dman->show();
    sleepM.cyclic();
  }

}
