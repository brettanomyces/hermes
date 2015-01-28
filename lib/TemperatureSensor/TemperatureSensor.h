#ifndef _HERMES_TEMP_SENSOR_LIB_
#define _HERMES_TEMP_SENSOR_LIB_

#include <Arduino.h>

class TemperatureSensor {

	public:

		TemperatureSensor();

		double readTemperature(int pin);

	private:

		// Variables

		// Constants
		int thermistorPos = 2;
		int dividerRes = 10000;
		
		double calculateThermistorResistance(int pin);
		double analogToVoltage(double analog);
		double voltageToResistance(double vOut);
		double calculateR1(double vOut);
		double calculateR2(double vOut);
		double temperatureToResistance(double t);
		double resistanceToTemperature(double Rt);
};

#endif
