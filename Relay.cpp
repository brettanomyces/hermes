#include "Relay.h"

Relay::Relay(int pin, double delay, DeviceManager* deviceManager) : m_delay(delay) {
  m_pin = pin;
  m_on = false;
  m_dm = deviceManager;
  m_delay.reset();
}

void Relay::activate() {
  if (!m_on) {
    if (m_delay.ok()) {
      m_dm->activateRelay(m_pin);
      m_on = true;
      m_delay.reset();
    }
  }
}

void Relay::deactivate() {
  if (m_on) {
    if (m_delay.ok()) {
      m_dm->deactivateRelay(m_pin);
      m_on = false;
      m_delay.reset();
    }
  }
}

bool Relay::isActive() {
  return m_on;
}

bool Relay::isWaiting() {
  if (m_delay.ok()) {
    return false;
  } else {
    return true;
  }
}
