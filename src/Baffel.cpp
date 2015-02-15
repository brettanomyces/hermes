#include "Baffel.h"

Baffel::Baffel(	int ena, int in1, int in2, int in3, int in4, int enb, int transformerPin ) : 
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
	// set to true so baffel will be closed on setup
	m_open = true;
}

void Baffel::open(){
	// check that the baffel is not already open
	if(!isOpen()){
		m_stepper.step(6 * STEPS);
		delay(TRAVEL_TIME);
		m_open = true;
	}
}

void Baffel::close(){
	if(isOpen()){
		m_stepper.step(-6 * STEPS);
		delay(TRAVEL_TIME);
		m_open = false;
	}
}

bool Baffel::isOpen(){
	return m_open;
}
