#include "DisplayRequest.h"

class DisplayManager
{
  DisplayRequest *request = 0;
public:
  virtual void requestDisplay(DisplayRequest *DispReq)
  {
    if(request == 0)
    {
      request = DispReq;
    }
    else if(&request < &DispReq)
    {
      request = DispReq;
    }
  }
  virtual void show();
};
