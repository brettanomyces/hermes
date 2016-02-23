#include <Relay.h>

Relay::Relay(int pin, double delay, DeviceManager deviceManager) : m_delay(delay) {
  m_pin = pin;
  m_on = false;
  m_delay.reset();
  m_dm = deviceManager;
}

void Relay::activate() {
  if (!m_on) {
    if (m_delay.ok()) {
      m_dm.activate(m_pin);
      m_on = true;
      m_delay.reset();
    }
  }
}

void Relay::deactivate() {
  if (m_on) {
    if (m_delay.ok()) {
      m_dm.deactivate(m_pin);
      m_on = false;
      m_delay.reset();
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
