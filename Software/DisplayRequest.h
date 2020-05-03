#ifndef __DISPLAYREQUEST_HEADER_INCLUDE__
#define __DISPLAYREQUEST_HEADER_INCLUDE__
typedef enum
{
  Empty,
  FadeIn,
  Time,
  SetMinute,
  SetHour,
  SetCorrMinute,
  SetCorrHour,
  SetCorrDay,
  SetCorrMonth,
  SetTempCorr,
  ShowTemperature,
  ShowStepsHistory,
  ShowUBatt,
  Debouncing,
  showStepCounter
} DisplayRequestType;

class DisplayRequest
{
public:
    static const uint8_t dataLength = 5;
private:
  DisplayRequestType type = Empty;
  uint16_t data[DisplayRequest::dataLength] = {0};
public:
  DisplayRequest():type(Empty){};
  DisplayRequest(DisplayRequestType drt, uint16_t dp[]):type(drt)
  {
    for(uint8_t i =0;i<DisplayRequest::dataLength;++i)
      data[i]=dp[i];
  }
  uint16_t *getData(){return data;}
  DisplayRequestType getType(){return type;}
  bool operator <(const DisplayRequest &b) const {return this->type < b.type;};
  bool operator >(const DisplayRequest &b) const {return this->type > b.type;};
  uint16_t operator[](uint8_t i){return data[i];}
};
#endif //__DISPLAYMANAGER_HEADER_INCLUDE__
