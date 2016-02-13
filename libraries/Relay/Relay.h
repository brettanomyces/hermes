#ifndef SRC_RELAY_H_
#define SRC_RELAY_H_

#include <Delay.h>

class Relay {
  public:
    // use default parameters
    explicit Relay(double delay = 0);

    void on();
    void off();
    bool isOn();
    bool isWaiting();

  private:
    bool m_on;
    Delay m_delay;
};

#endif  // SRC_RELAY_H_
