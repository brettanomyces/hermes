#ifndef SRC_RELAY_H_
#define SRC_RELAY_H_

#include <Arduino.h>
#include <Delay.h>

class Relay {
  public:
    // use default parameters
    explicit Relay(int pin, String name = "", double delay = 0);

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

#endif  // SRC_RELAY_H_
