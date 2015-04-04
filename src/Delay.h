#ifndef SRC_DELAY_H_
#define SRC_DELAY_H_
#include <Arduino.h>

class Delay {
  public:
    explicit Delay(double period);
    void reset();
    bool ok();

  private:
    double m_period;
    double m_lastTime;
};
#endif  // SRC_DELAY_H_
