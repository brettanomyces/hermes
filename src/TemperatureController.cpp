#include <Arduino.h>
#include "TemperatureController.h"

TemperatureController::TemperatureController(
		Baffel& baffel,
		Relay& compressorRelay,
		Relay& fanRelay,
		Relay& heaterRelay,
		TemperatureSensor& freezerSensor,
		TemperatureSensor& fridgeSensor
		):
		m_baffel(baffel),
		m_compressorRelay(compressorRelay),
		m_fanRelay(fanRelay),
		m_heaterRelay(heaterRelay),
		m_freezerSensor(freezerSensor),
		m_fridgeSensor(fridgeSensor)
		{
	m_fridgeSetValue = 20.0;
	m_freezerSetValue = 10.0;
	m_differenceSetValue = 0.5;
	m_compressorDelayTime = 18000; //millis
};

void TemperatureController::setFridgeTemperature(double temperature){
	m_fridgeSetValue = temperature;
}

void TemperatureController::setFreezerTemperature(double temperature){
	m_freezerSetValue = temperature;
}

void TemperatureController::setCompressorDelayTime(unsigned long millis){
	m_compressorDelayTime = millis;
}

void TemperatureController::setDifference(double degrees){
	m_differenceSetValue = degrees;
}

void TemperatureController::maintainTemperature(){
	double currentFridgeTemperature = m_fridgeSensor.readTemperature();
	double currentFreezerTemperature = m_freezerSensor.readTemperature();

	if (currentFridgeTemperature > m_fridgeSetValue + m_differenceSetValue) {
		// to hot
		m_heaterRelay.off();
		m_baffel.open();
		m_fanRelay.on();
	} else if (currentFridgeTemperature < m_fridgeSetValue - m_differenceSetValue){
		// to cold
		m_heaterRelay.on();
		m_baffel.close();
		if (!m_compressorRelay.isOn()){
			m_fanRelay.off();
		}
	} else {
		// temp in ok range
		m_heaterRelay.off();
		m_baffel.close();
		if (!m_compressorRelay.isOn()){
			m_fanRelay.off();
		}
	}

	if (currentFreezerTemperature > m_freezerSetValue + m_differenceSetValue){
		// to hot
		// wait m_differenceSetValue after turning off compressor to turn it back on
		unsigned long currentMillis = millis();
		if ((unsigned long)(currentMillis - m_compressorTurnedOff) >= m_compressorDelayTime) {
			m_compressorRelay.on();
			m_fanRelay.on();
		}
	} else if (currentFreezerTemperature < m_freezerSetValue - m_differenceSetValue){
		// to cold
		if(m_compressorRelay.isOn()){
			unsigned long m_compressorTurnedOff = millis();
			m_compressorRelay.off();
		}
		if(!m_baffel.isOpen()){
			m_fanRelay.off();
		}
		// We don't worry about heating the freezer section as it doesn't really
		// matter if its a bit too cold and we save power.
	} else {
		// temp in ok range
		if(m_compressorRelay.isOn()){
			unsigned long m_compressorTurnedOff = millis();
			m_compressorRelay.off();
		}
		if(!m_baffel.isOpen()){
			m_fanRelay.off();
		}
	}
}
