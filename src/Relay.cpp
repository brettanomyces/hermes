#include <Arduino.h>
#include "Relay.h"

Relay::Relay(int pin){
	m_pin = pin;
	pinMode(m_pin, OUTPUT);
	off();
}

void Relay::on(){
	digitalWrite(m_pin, HIGH);
}

void Relay::off(){
	digitalWrite(m_pin, LOW);
}



