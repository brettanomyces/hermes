
#ifndef _HERMES_BAFFEL_
#define _HERMES_BAFFEL_

#include <Arduino.h>
#include <Stepper.h>
#include "Relay.h"

class Baffel {
	public:
		Baffel( int ena,
			int in1,
			int in2,
			int in3,
			int in4,
			int enb,
			int transformerPin);

		void open();
		void close();
		bool isOpen();

	private:
		int m_ena;
		int m_enb;
		bool m_open;
		Stepper m_stepper;
		Relay m_transformer;

		static const int TRAVEL_TIME = 4000;
		static const int STEPS = 300;
		static const int SPEED = 100;

		void enable();
		void disable();
};
#endif
