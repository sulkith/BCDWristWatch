#ifndef __DEBOUNCE_HEADER_INCLUDE__
#define __DEBOUNCE_HEADER_INCLUDE__
#include <inttypes.h>
#include <stddef.h>
template <typename T>
class Debouncer
{
  private:
    const uint16_t debTime;
    T currentValue;
    T currentDebouncedValue;
    Debouncer(){};
    uint16_t time;
    T (*pollingFunction)();
    uint8_t updated;
  public:
    Debouncer(uint16_t DebounceTime, T initialValue):
    debTime(DebounceTime), currentValue(initialValue),
    currentDebouncedValue(initialValue),time(DebounceTime),
    updated(0)
    {pollingFunction = NULL;};

    Debouncer(uint16_t DebounceTime, T initialValue, T (*pollingFunc)()):
    debTime(DebounceTime), currentValue(initialValue),
    currentDebouncedValue(initialValue),time(DebounceTime),
    updated(0)
    {pollingFunction=pollingFunc;};

    T getValue(uint16_t dT);
    void setValue(T Value);
    void loop(uint16_t dT);
    void directSetValue(T Value, uint8_t setUpdated = 0);

    inline T getValue()
    {
      updated = 0;
      return currentDebouncedValue;
    };

    inline uint8_t valueUpdated()
    {
      return updated;
    };
    uint8_t valueUpdatedTo(T Value)
    {
      if(updated == 1 && currentDebouncedValue == Value)
      {
        updated = 0;
        return 1;
      }
      return 0;
    };
};

template <typename T> T Debouncer<T>::getValue(uint16_t dT)
{
  this.loop(dT);
  return this.getValue();
}
template <typename T> void Debouncer<T>::setValue(T Value)
{
  if(currentValue!=Value)
  {
    currentValue = Value;
    time = 0;
  }
}
template <typename T> void Debouncer<T>::loop(uint16_t dT)
{
  if(pollingFunction!=NULL)
  {
    setValue((*pollingFunction)());
  }
  if(currentDebouncedValue != currentValue)
  {
    if(time<debTime)
    {
      time += dT;
    }
    else
    {
      currentDebouncedValue = currentValue;
      updated = 1;
    }
  }
}
template <typename T> void Debouncer<T>::directSetValue(T Value, uint8_t setUpdated)
{
  time=debTime;
  currentValue=Value;
  currentDebouncedValue=currentValue;
  updated = setUpdated;
}
#endif //__DEBOUNCE_HEADER_INCLUDE__
