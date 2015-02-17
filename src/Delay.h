#ifndef _HERMES_DELAY_
#define _HERMES_DELAY_

#include <Arduino.h> 

class Delay
{
public:
  Delay(long);
  void reset();
  bool ok();
private:
  unsigned long period;
  unsigned long lastTime;
};

#endif
