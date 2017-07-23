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
    reverse(in1, in2, in3, in4, speed);
  }
}

void DeviceManager::closeBaffel(int in1, int in2, int in3, int in4, int steps, int speed) {
  for (int i = 0; i <= steps; i++ ) {
    forward(in1, in2, in3, in4, speed);
  }
}

void DeviceManager::forward(int in1, int in2, int in3, int in4, int speed) {
  // 1
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);

  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
  delay(speed);

  // 2
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);

  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  delay(speed);

  // 3
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);

  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  delay(speed);

  // 4
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);

  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
  delay(speed);
}

void DeviceManager::reverse(int in1, int in2, int in3, int in4, int speed) {
  //1
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);

  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
  delay(speed);  

  // 2
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);

  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
  delay(speed);

  // 3
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);

  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  delay(speed);

  // 4
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);

  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  delay(speed);
}

double DeviceManager::readAnalogValue(int pin){
  double sum = 0;
  int i;
  for (i = 0; i < NUM_REPITIONS; i++) {
    sum += static_cast<double>(analogRead(pin));
  }
  double analog =  sum / static_cast<double>(NUM_REPITIONS);
  return analog;
}
 
unsigned long DeviceManager::getMillis(){
  return millis();
}
