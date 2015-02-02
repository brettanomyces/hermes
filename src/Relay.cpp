#include <Arduino.h>
#include "Relay.h"

Relay::Relay(int pin){
	m_pin = pin;
	pinMode(m_pin, OUTPUT);
	off();
}

void Relay::on(){
	digitalWrite(m_pin, LOW);
	m_on = true;
}

void Relay::off(){
	digitalWrite(m_pin, HIGH);
	m_on = false;
}

bool Relay::isOn(){
	return m_on;
}


