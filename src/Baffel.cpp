#include <Arduino.h>
#include "Baffel.h"

Baffel::Baffel(int ena, int in1, int in2, int in3, int in4, int enb)
	: m_stepper(STEPS, in1, in2, in3, in4) {
	
	pinMode(ena, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	pinMode(enb, OUTPUT);

	m_ena = ena;
	m_enb = enb;
	m_stepper.setSpeed(SPEED);
}

void Baffel::open(){
	enable();
	m_stepper.step(6 * STEPS);
	delay(TRAVEL_TIME);
	disable();
	m_open = true;
}

void Baffel::close(){
	enable();
	m_stepper.step(-6 * STEPS);
	delay(TRAVEL_TIME);
	disable();
	m_open = false;
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
