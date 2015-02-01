#ifndef _HERMES_TEMP_CONTROLLER_
#define _HERMES_TEMP_CONTROLLER_

#include "Baffel.h"
#include "Relay.h"
#include "TemperatureSensor.h"

class TemperatureController {
	public:
		TemperatureController(
			Baffel& baffel,
			Relay& compressorRelay,
			Relay& fanRelay,
			Relay& heaterRelay,
			TemperatureSensor& freezerSensor,
			TemperatureSensor& fridgeSensor
		);

		void maintainTemperature();
		void setCompressorDelayTime(unsigned long millis);
		void setDifference(double degrees);
		void setFreezerTemperature(double temperature);
		void setFridgeTemperature(double temperature);

	private:
		double m_fridgeSetValue;
		double m_freezerSetValue;
		double m_differenceSetValue;
		unsigned long m_compressorDelayTime; // millis
		unsigned long m_compressorTurnedOff;

		Baffel& m_baffel;
		Relay& m_compressorRelay;
		Relay& m_fanRelay;
		Relay& m_heaterRelay;
		TemperatureSensor& m_freezerSensor;
		TemperatureSensor& m_fridgeSensor;
};

#endif
