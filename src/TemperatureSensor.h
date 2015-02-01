#ifndef _HERMES_TEMP_SENSOR_
#define _HERMES_TEMP_SENSOR_

class TemperatureSensor {

	public:
		// Constructor
		TemperatureSensor(int pin, int thermistorPosition, int resistorValue);

		double readTemperature();

	private:
		// Variables
		int m_pin;
		int m_thermistorPosition;
		int m_resistorValue;

		// Constants
		static const double V_IN = 5.0;
		
		double measureVoltage(int pin);
		double analogToVoltage(double analog);
		double voltageToResistance(double vOut);
		double calculateR1(double vOut);
		double calculateR2(double vOut);
		double temperatureToResistance(double t);
		double resistanceToTemperature(double Rt);
};

#endif
