#include "Relay.h"

Relay::Relay(int pin, long delay) : m_delay(delay){
  m_pin = pin;
  pinMode(m_pin, OUTPUT);
  // start in off state
  digitalWrite(m_pin, HIGH);
  m_on = false;
  // reset delay
  m_delay.reset();
}

Relay::Relay(int pin) : m_delay(0) {
  m_pin = pin;
  pinMode(m_pin, OUTPUT);
  // start in off state
  digitalWrite(m_pin, HIGH);
  m_on = false;
}

void Relay::on(){
  if(!m_on){
    if(m_delay.ok()){
      digitalWrite(m_pin, LOW);
      Serial.print(m_pin);
      m_on = true;
      m_delay.reset();
      Serial.print(m_pin);
      Serial.println(" turned on");
    } else {
      Serial.print(m_pin);
      Serial.println(" waiting to turn on");
    }
  } else {
    //Serial.print(m_pin);
    //Serial.println(" already on");
  }
}

void Relay::off(){
  if(m_on){
    if(m_delay.ok()){
      digitalWrite(m_pin, HIGH);
      Serial.print(m_pin);
      m_on = false;
      m_delay.reset();
      Serial.print(m_pin);
      Serial.println(" turned off");
    } else {
      Serial.print(m_pin);
      Serial.println(" waiting to turn off");
    }
  } else {
    //Serial.print(m_pin);
    //Serial.println(" already off");
  }
}

bool Relay::isOn(){
  return m_on;
}

bool Relay::waiting(){
  if(m_delay.ok()){
    return false;
  } else {
    return true;
  }
}
