#include <Delay.h>

Delay::Delay(double period) {
  m_period = period;
  m_lastTime = 0;
}

// reset the delay period
void Delay::reset() {
  m_lastTime = millis();
}

// check if delay period has been exceeded
// true if delay has been exceeded
bool Delay::ok() {
  if (millis() - m_lastTime > m_period) {
    // note that unlike DoEvery's check() we do not increment lastTime
    return true;
  } else {
    return false;
  }
}
