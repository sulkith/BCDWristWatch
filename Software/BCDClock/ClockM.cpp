#include "ClockM.h"

#include "DisplayManager.h" //only for testing

ClockM ClockM::instance(23,59,50);
EEPM DummyEEPM;

ClockM::ClockM(uint8_t h, uint8_t m, uint8_t s)
{
  t.hour = h;
  t.minute = m;
  t.second = s;
  eepm_ptr = &DummyEEPM;
}
void ClockM::setEEPM(EEPM *ptr)
{
  //delete eepm_ptr;
  eepm_ptr = ptr;
}
uint8_t ClockM::operator++()
{
  hour_changed = false;
  ctr++;
  if (++t.second==60)        //keep track of time, date, month, and year
  {
    t.second=eepm_ptr->getCorrEveryMinute();
    if (++t.minute==60)
    {
      t.minute=0;
      t.second+=eepm_ptr->getCorrEveryHour();
      hour_changed = true;
      if (++t.hour==24)
      {
        t.hour=0;
        t.second+=eepm_ptr->getCorrEveryDay();;
        /*uint16_t temp_raw = octs.get();
        if(temp_raw!=0xFFFF)
        {
          int16_t temp_raw2 = temp_raw - tempCorrectionOffset;
          if(temp_raw2<0) temp_raw2 = -temp_raw2;
          t.steps+=tempCorrection[temp_raw2%50];//%50 so we dont get a segfault
          t.second += t.steps/10000;
          t.steps = t.steps % 10000;
        }*/
        if(++t.day==30)
        {
          t.second+=eepm_ptr->getCorrEveryMonth();;
          t.day=0;
        }
        if(t.second>59)
        {
          t.minute+=t.second/60;
          t.second=t.second%60;
        }
      }
    }
  }
  return t.minute;
}
uint8_t ClockM::advanceMinute()
{
  t.second = 0;
  t.day=0;
  return t.minute = (t.minute + 1)%60;
}
uint8_t ClockM::advanceHour()
{
  t.second = 0;
  t.day=0;
  return t.hour = (t.hour + 1)%24;

}
