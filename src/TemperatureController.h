#ifndef _HERMES_TEMP_CONTROLLER_
#define _HERMES_TEMP_CONTROLLER_

#include "Baffel.h"
#include "Relay.h"
#include "TemperatureSensor.h"

// Forward Declaration
class Baffel;
class Relay;
class TemperatureSensor;

class TemperatureController {
	public:
		TemperatureController(
			Baffel& baffel,
			Relay& compressorRelay,
			Relay& fanRelay,
			Relay& heaterRelay,
			TemperatureSensor& freezerTempSenor,
			TemperatureSensor& fridgeTempSensor
		);

		void maintainTemperature();
		void setCompressorDelayTime(unsigned long seconds);
		void setDifference(double temperature);
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
