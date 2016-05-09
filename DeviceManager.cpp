#include "DeviceManager.h"

DeviceManager::DeviceManager(){
}

void DeviceManager::activateRelay(int pin) {
  digitalWrite(pin, m_active);
}

void DeviceManager::deactivateRelay(int pin) {
  digitalWrite(pin, !m_active);
}

void DeviceManager::openBaffel(int in1, int in2, int in3, int in4, int speed, int steps){
  for (int i = 0; i <= steps; i++ ) {
    clockwise(in1, in2, in3, in4);
    delay(speed);
  }
}

void DeviceManager::closeBaffel(int in1, int in2, int in3, int in4, int speed, int steps){
  for (int i = 0; i <= steps; i++ ) {
    antiClockwise(in1, in2, in3, in4);
    delay(speed);
  }
}

void DeviceManager::clockwise(int in1, int in2, int in3, int in4) {
  // 1
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  // 2
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  // 3
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  // 4
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void DeviceManager::antiClockwise(int in1, int in2, int in3, int in4) {
  // 1
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);

  // 2
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);

  // 3
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);

  // 4
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in2, LOW);
}
