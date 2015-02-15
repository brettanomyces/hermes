#include "Relay.h"

Relay::Relay(int pin){
	m_pin = pin;
	pinMode(m_pin, OUTPUT);
	// start in off state
	digitalWrite(m_pin, HIGH);
	m_on = false;
}

void Relay::on(){
	// check that relay is not already on
	if(!isOn()){
		digitalWrite(m_pin, LOW);
		Serial.print(m_pin);
		Serial.println(" turned on");
	} else {
		Serial.print(m_pin);
		Serial.println(" already on");
	}
	m_on = true;
}

void Relay::off(){
	if(isOn()){
		digitalWrite(m_pin, HIGH);
		Serial.print(m_pin);
		Serial.println(" turned off");
	} else {
		Serial.print(m_pin);
		Serial.println(" already off");
	}
	m_on = false;
}

bool Relay::isOn(){
	return m_on;
}
