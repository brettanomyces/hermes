#include "Relay.h"

// delegating constructors
Relay::Relay(int pin) : Relay(pin, "relay", 0){} // default name: "relay", default delay: 0
Relay::Relay(int pin, String name, long delay) : m_delay(delay){
  m_pin = pin;
  m_name = name;
  pinMode(m_pin, OUTPUT);
  // start in off state
  digitalWrite(m_pin, HIGH);
  m_on = false;
  // reset delay
  m_delay.reset();
}

void Relay::on(){
  if(!m_on){
    if(m_delay.ok()){
      digitalWrite(m_pin, LOW);
      m_on = true;
      m_delay.reset();
      log("turned on");
    } else {
      log("waiting to turn on");
    }
  }
}

void Relay::off(){
  if(m_on){
    if(m_delay.ok()){
      digitalWrite(m_pin, HIGH);
      m_on = false;
      m_delay.reset();
      log("turned off");
    } else {
      log("waiting to turn off");
    }
  }
}

void Relay::log(String str){
  Serial.print(m_name);
  Serial.print("(");
  Serial.print(m_pin);
  Serial.print("): ");
  Serial.print(str);
  Serial.println();
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
