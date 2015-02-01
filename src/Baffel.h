
#ifndef _HERMES_BAFFEL_
#define _HERMES_BAFFEL_

#include <Arduino.h>
#include <Stepper.h>

class Baffel {
	public:
		Baffel(int ena, int in1, int in2, int in3, int in4, int enb);
		
		void open();
		void close();

	private:
		int m_ena;
		int m_enb;
		Stepper m_stepper;

		static const int STEPS = 300;
		static const int SPEED = 100;

		void enable();
		void disable();
};
#endif
