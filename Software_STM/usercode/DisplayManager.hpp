#ifndef __DISPLAYMANAGER_HEADER_INCLUDE__
#define __DISPLAYMANAGER_HEADER_INCLUDE__
#include "DisplayRequest.hpp"

class DisplayManager
{
protected:
  DisplayRequest request;
  uint8_t Brightness = 255;
public:
  DisplayManager(){}
  virtual void requestDisplay(DisplayRequest DispReq)
  {
    if(request.getType() == Empty)
    {
      request = DispReq;
    }
    else if(request < DispReq)
    {
      request = DispReq;
    }
  }
  virtual void show() = 0;
  virtual void showERROR(uint8_t error, uint8_t data) {
	uint16_t d[DisplayRequest::dataLength];
	d[0] = error;
	d[1] = data;
	DisplayRequest dr(showError,d);
  	requestDisplay(dr);
  	show();
  }
  virtual void init() = 0;
  virtual void setBrightness(uint8_t brightness)
  {
	  Brightness = brightness;
  }
};
#endif //__DISPLAYMANAGER_HEADER_INCLUDE__
