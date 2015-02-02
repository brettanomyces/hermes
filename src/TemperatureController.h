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
			TemperatureSensor& fzSensor,
			TemperatureSensor& frSensor
		);

		void maintainTemperature();
		void setCompressorDelayTime(unsigned long millis);
		void setDifference(double degrees);
		void setFzSetTemp(double temp);
		double getFzSetTemp();
		void setFrSetTemp(double temp);
		double getFrSetTemp();

	private:
		double m_frSetTemp;
		double m_fzSetTemp;
		double m_diff;
		unsigned long m_compressorDelayTime; // millis
		unsigned long m_compressorTurnedOff;

		Baffel& m_baffel;
		Relay& m_compressorRelay;
		Relay& m_fanRelay;
		Relay& m_heaterRelay;
		TemperatureSensor& m_fzSensor;
		TemperatureSensor& m_frSensor;
};

#endif
