#include "Delay.h"

Delay::Delay(double period, IDeviceManager* deviceManager) {
  m_period = period;
  m_dm = deviceManager;
  m_lastTime = 0;
}

// reset the delay period
void Delay::reset() {
  m_lastTime = m_dm->getMillis();
}

// check if delay period has been exceeded
// true if delay has been exceeded
bool Delay::ok() {
  if (m_dm->getMillis() - m_lastTime >= m_period) {
    // note that unlike DoEvery's check() we do not increment lastTime
    return true;
  } else {
    return false;
  }
}
