#include "DeviceManager.h"

// set all register pins to LOW
void DeviceManager::clearRegisters(){
  for(int i = NUM_SHIFT_REGISTER_PINS - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 

// NOTE: only call AFTER all values are set
void DeviceManager::writeRegisters(){

  digitalWrite(LATCH_PIN, LOW);

  for(int i = NUM_SHIFT_REGISTER_PINS - 1; i >=  0; i--){
    digitalWrite(CLOCK_PIN, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(CLOCK_PIN, HIGH);

  }
  digitalWrite(LATCH_PIN, HIGH);

}

//set an individual pin HIGH or LOW
void DeviceManager::setRegisterPin(int index, int value){
  registers[index] = value;
}


void DeviceManager::turnOnRelay(int pin) {
	setRegisterPin(pin, active);
	writeRegisters();
}

void DeviceManager::turnOffRelay(int pin) {

}

void DeviceManager::openBaffel(int in1, int in2, int in3, int in4){

}

void DeviceManager::closeBaffel(int in1, int in2, int in3, int in4){

}


void clockwise() {
  // 1
  setRegisterPin(STEPPER_IN1, HIGH);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN4, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 2
  setRegisterPin(STEPPER_IN1, LOW);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN4, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 3
  setRegisterPin(STEPPER_IN1, LOW);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN4, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 4
  setRegisterPin(STEPPER_IN1, HIGH);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN4, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
}

void antiClockwise() {
  // 1
  setRegisterPin(STEPPER_IN4, HIGH);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN1, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 2
  setRegisterPin(STEPPER_IN4, LOW);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN1, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 3
  setRegisterPin(STEPPER_IN4, LOW);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN1, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 4
  setRegisterPin(STEPPER_IN4, HIGH);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN1, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
}
