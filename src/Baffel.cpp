#include "Baffel.h"

Baffel::Baffel(  int ena, int in1, int in2, int in3, int in4, int enb, int transformerPin ) : 
  m_stepper(STEPS, in1, in2, in3, in4), m_transformer(transformerPin) {

  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);

  m_ena = ena;
  m_enb = enb;
  m_stepper.setSpeed(SPEED);
  m_open = true;
}

// attempt to open baffel. does nothing if baffel is already open
void Baffel::open(){
  if(!isOpen()){
    forceOpen();
  } 
}

// attempt to close baffel. does nothing if baffel is already closed
void Baffel::close(){
  if(isOpen()){
    forceClose();
  }
}

// attempt to open baffel without checking if baffel is already open
void Baffel::forceOpen(){
  enable();
  m_stepper.step(6 * STEPS);
  delay(TRAVEL_TIME);
  disable();
  m_open = true;
  Serial.println("baffel opened");
}

// attempt to close baffel without checking if baffel is already closed
void Baffel::forceClose(){
  enable();
  m_stepper.step(-6 * STEPS);
  delay(TRAVEL_TIME);
  disable();
  m_open = false;
  Serial.println("baffel closed");
}

void Baffel::enable(){
  digitalWrite(m_ena, HIGH);
  digitalWrite(m_enb, HIGH);
}

void Baffel::disable(){
  digitalWrite(m_ena, LOW);
  digitalWrite(m_enb, LOW);
}

bool Baffel::isOpen(){
  return m_open;
}
