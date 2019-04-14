#include "EEPM.h"
#include "ClockM.h"
#include "DisplayManager.h"
#include "DisplayRequest.h"
#include "HAL.h"
#include "SleepM.h"

#include "BinaryWatch.h"
#include "EEPM_AVR.h"
#include "TwoButtonUI.h"

void init();

int main(void)
{
  EEPM_AVR eep;
  BinaryWatch hal;
  TwoButtonUI UI(&hal, &hal, &hal);
  SleepM sleepM(&hal);
  sleepM.subscribe(&UI);
  sleepM.subscribe(&eep);

  hal.HAL_init();
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
        hal.requestDisplay(drsethr);
        hal.show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        hal.requestDisplay(drsetmn);
        hal.show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        hal.requestDisplay(drsetmc);
        hal.show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        hal.requestDisplay(drsethc);
        hal.show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        hal.requestDisplay(drsetdc);
        hal.show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        hal.requestDisplay(drsetmoc);
        hal.show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        hal.requestDisplay(drsettc);
        hal.show();
      }
      for(uint16_t i =0;i<1000;++i)
      {
        hal.requestDisplay(drst);
        hal.show();
      }
    }
  }

  while(1)
  {
    hal.HAL_cyclic();
    UI.cyclic();
    hal.show();
    sleepM.cyclic();
  }

}
