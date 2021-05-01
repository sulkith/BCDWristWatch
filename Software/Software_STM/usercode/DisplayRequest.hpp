#ifndef __DISPLAYREQUEST_HEADER_INCLUDE__
#define __DISPLAYREQUEST_HEADER_INCLUDE__

#include "main.h"
#include "string.h" //memcpy is needed
typedef enum
{
  Empty,
  FadeIn,
  Time,
  Time_s,
  Date,
  SetMinute,
  SetHour,
  SetDay,
  SetMonth,
  SetYear,
  ShowStepsHistory,
  ShowUBatt,
  Debouncing,
  DebouncingDate,
  showStepCounter,
  showError,
  Raw_Output
} DisplayRequestType;

class DisplayRequest
{
public:
    static const uint8_t dataLength = 6;
private:
  DisplayRequestType type __attribute__ ((aligned (16))) = Empty;
  uint16_t data[DisplayRequest::dataLength] __attribute__ ((aligned (16))) = {0} ;
public:
  DisplayRequest(){};
  DisplayRequest(DisplayRequestType drt, uint16_t dp[DisplayRequest::dataLength]):type(drt)
  {
    for(uint8_t i =0;i<DisplayRequest::dataLength;++i)
    {
//      data[i]=dp[i];
      memcpy(data,dp,dataLength*sizeof(uint16_t));
    }
  }
  uint16_t *getData(){return (uint16_t*)data;}
  DisplayRequestType getType(){return type;}
  bool operator <(const DisplayRequest &b) const {return this->type < b.type;};
  bool operator >(const DisplayRequest &b) const {return this->type > b.type;};
  uint16_t operator[](uint8_t i){return data[i];}
};
#endif //__DISPLAYMANAGER_HEADER_INCLUDE__
