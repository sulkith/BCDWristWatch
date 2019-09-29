#ifndef __DISPLAYMANAGER_HEADER_INCLUDE__
#define __DISPLAYMANAGER_HEADER_INCLUDE__
#include "DisplayRequest.h"

class DisplayManager
{
protected:
  DisplayRequest request;
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
  virtual void show();
};
#endif //__DISPLAYMANAGER_HEADER_INCLUDE__
