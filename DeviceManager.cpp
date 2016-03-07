#include "DeviceManager.h"

DeviceManager::DeviceManager(int data, int latch, int clock){
  m_data = data;
  m_latch = latch;
  m_clock = clock;
}

// set all register pins to LOW
void DeviceManager::clearRegisters(){
  for(int i = NUM_SHIFT_REGISTER_PINS - 1; i >=  0; i--){
    m_registers[i] = LOW;
  }
} 

// NOTE: only call AFTER all values are set
void DeviceManager::writeRegisters(){

  digitalWrite(m_latch, LOW);

  for(int i = NUM_SHIFT_REGISTER_PINS - 1; i >=  0; i--){
    digitalWrite(m_clock, LOW);

    int val = m_registers[i];

    digitalWrite(m_data, val);
    digitalWrite(m_clock, HIGH);

  }
  digitalWrite(m_latch, HIGH);

}

//set an individual pin HIGH or LOW
void DeviceManager::setRegisterPin(int index, int value){
  m_registers[index] = value;
}


void DeviceManager::activateRelay(int pin) {
  setRegisterPin(pin, m_active);
  writeRegisters();
}

void DeviceManager::deactivateRelay(int pin) {
  setRegisterPin(pin, !m_active);
  writeRegisters();

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
  setRegisterPin(in1, HIGH);
  setRegisterPin(in2, LOW);
  setRegisterPin(in3, HIGH);
  setRegisterPin(in4, LOW);
  writeRegisters(); 
  // 2
  setRegisterPin(in1, LOW);
  setRegisterPin(in2, HIGH);
  setRegisterPin(in3, HIGH);
  setRegisterPin(in4, LOW);
  writeRegisters(); 
  // 3
  setRegisterPin(in1, LOW);
  setRegisterPin(in2, HIGH);
  setRegisterPin(in3, LOW);
  setRegisterPin(in4, HIGH);
  writeRegisters(); 
  // 4
  setRegisterPin(in1, HIGH);
  setRegisterPin(in2, LOW);
  setRegisterPin(in3, LOW);
  setRegisterPin(in4, HIGH);
  writeRegisters(); 
}

void DeviceManager::antiClockwise(int in1, int in2, int in3, int in4) {
  // 1
  setRegisterPin(in4, HIGH);
  setRegisterPin(in3, LOW);
  setRegisterPin(in2, HIGH);
  setRegisterPin(in1, LOW);
  writeRegisters(); 
  // 2
  setRegisterPin(in4, LOW);
  setRegisterPin(in3, HIGH);
  setRegisterPin(in2, HIGH);
  setRegisterPin(in1, LOW);
  writeRegisters(); 
  // 3
  setRegisterPin(in4, LOW);
  setRegisterPin(in3, HIGH);
  setRegisterPin(in2, LOW);
  setRegisterPin(in1, HIGH);
  writeRegisters(); 
  // 4
  setRegisterPin(in4, HIGH);
  setRegisterPin(in3, LOW);
  setRegisterPin(in2, LOW);
  setRegisterPin(in1, HIGH);
  writeRegisters(); 
}
