#include "DeviceManager.h"

DeviceManager::DeviceManager(){
}

void DeviceManager::activateRelay(int pin, bool activeLow) {
  if (activeLow) {
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH);
  }
}

void DeviceManager::deactivateRelay(int pin, bool activeLow) {
  if (activeLow) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

unsigned long DeviceManager::getMillis(){
  return millis();
}
