#include "Relay.h"

Relay::Relay(int pin, double delay, bool activeLow, IDeviceManager* deviceManager) : m_delay(delay, deviceManager) {
  m_pin = pin;
  m_active = false;
  m_activeLow = activeLow;
  m_dm = deviceManager;
  m_delay.reset();
}

void Relay::activate() {
  m_dm->activateRelay(m_pin, m_activeLow);
  m_active = true;
  m_delay.reset();
}

void Relay::deactivate() {
  m_dm->deactivateRelay(m_pin, m_activeLow);
  m_active = false;
  m_delay.reset();
}

bool Relay::isActive() {
  return m_active;
}

bool Relay::isWaiting() {
  if (m_delay.ok()) {
    return false;
  } else {
    return true;
  }
}
