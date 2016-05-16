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

void DeviceManager::openBaffel(int in1, int in2, int in3, int in4, int steps, int speed) {
  for (int i = 0; i <= steps; i++ ) {
    clockwise(in1, in2, in3, in4, speed);
  }
}

void DeviceManager::closeBaffel(int in1, int in2, int in3, int in4, int steps, int speed) {
  for (int i = 0; i <= steps; i++ ) {
    antiClockwise(in1, in2, in3, in4, speed);
  }
}

void DeviceManager::clockwise(int in1, int in2, int in3, int in4, int speed) {
  // 1
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(speed);

  // 2
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(speed);

  // 3
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(speed);

  // 4
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(speed);
}

void DeviceManager::antiClockwise(int in1, int in2, int in3, int in4, int speed) {
  // note the order onf the inX has changed
  // 1
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  delay(speed);

  // 2
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  delay(speed);

  // 3
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  delay(speed);

  // 4
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  delay(speed);
}
 
unsigned long DeviceManager::getMillis(){
  return millis();
}
