#ifndef _HERMES_RELAY_
#define _HERMES_RELAY_

#include <Arduino.h>

class Relay {
	public:
		Relay(int pin);

		void on();
		void off();

	private:
		int m_pin;
};
#endif
