#ifndef _HERMES_RELAY_
#define _HERMES_RELAY_

#include <Arduino.h>

class Relay {
	public:
		Relay(int pin);

		void on();
		void off();
		bool isOn();

	private:
		int m_pin;
		bool m_on;
};
#endif
