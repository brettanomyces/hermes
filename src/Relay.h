#ifndef _HERMES_RELAY_
#define _HERMES_RELAY_

#include <Arduino.h>
#include "Delay.h"

class Relay {
  public:
    Relay(int pin);
    Relay(int pin, String name, long delay);

    void on();
    void off();
    void log(String str);
    bool isOn();
    bool waiting();

  private:
    int m_pin;
    String m_name;
    bool m_on;
    Delay m_delay;
};

#endif // _HERMES_RELAY_
