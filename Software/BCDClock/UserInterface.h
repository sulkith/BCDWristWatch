#ifndef __USERINTERFACE_HEADER_INCLUDE__
#define __USERINTERFACE_HEADER_INCLUDE__
class UserInterface
{
public:
  virtual void init(){};
  virtual void cyclic()=0;
};
#endif //__USERINTERFACE_HEADER_INCLUDE__
