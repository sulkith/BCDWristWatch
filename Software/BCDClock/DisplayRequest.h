#ifndef __DISPLAYREQUEST_HEADER_INCLUDE__
#define __DISPLAYREQUEST_HEADER_INCLUDE__
typedef enum
{
  Time,
  SetMinute,
  SetHour,
  SetCorrMinute,
  SetCorrHour,
  SetCorrDay,
  SetCorrMonth,
  SetTempCorr,
  ShowTemperature
} DisplayRequestType;

class DisplayRequest
{
  DisplayRequestType type;
  uint16_t data[4];
public:
  uint16_t *getData(){return data;}
  DisplayRequestType getType(){return type;}
  bool operator <(const DisplayRequest &b) const {return this->type < b.type;};
  bool operator >(const DisplayRequest &b) const {return this->type > b.type;};
};
#endif //__DISPLAYMANAGER_HEADER_INCLUDE__
