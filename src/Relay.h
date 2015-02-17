#ifndef _HERMES_RELAY_
#define _HERMES_RELAY_

#include <Arduino.h>
#include "Delay.h"


class Relay {
  public:
    Relay(int pin, long delay);
    Relay(int pin);

    void on();
    void off();
    bool isOn();
    bool waiting();

  private:
    int m_pin;
    bool m_on;
    Delay m_delay;
};
#endif
