#include <Relay.h>

Relay::Relay(double delay) : m_delay(delay) {
  m_on = false;
  m_delay.reset();
}

void Relay::on() {
  if (!m_on) {
    if (m_delay.ok()) {
      m_on = true;
      m_delay.reset();
    } else {
      // do nothing
    }
  }
}

void Relay::off() {
  if (m_on) {
    if (m_delay.ok()) {
      m_on = false;
      m_delay.reset();
    } else {
      // do nothing
    }
  }
}

bool Relay::isOn() {
  return m_on;
}

bool Relay::isWaiting() {
  if (m_delay.ok()) {
    return false;
  } else {
    return true;
  }
}
