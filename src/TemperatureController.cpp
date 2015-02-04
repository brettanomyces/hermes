#include <Arduino.h>
#include "TemperatureController.h"

TemperatureController::TemperatureController(
		Baffel& baffel,
		Relay& compressorRelay,
		Relay& fanRelay,
		Relay& heaterRelay,
		TemperatureSensor& fzSensor,
		TemperatureSensor& frSensor
		):
		m_baffel(baffel),
		m_compressorRelay(compressorRelay),
		m_fanRelay(fanRelay),
		m_heaterRelay(heaterRelay),
		m_fzSensor(fzSensor),
		m_frSensor(frSensor)
		{
	// default values
	m_frSetTemp = 20.0;
	m_fzSetTemp = 10.0;
	m_diff = 0.5;
	m_compressorDelayTime = 180000; // millis
	m_compressorTurnedOff = millis();
};

void TemperatureController::setFrSetTemp(double temp){
	m_frSetTemp = temp;
}

double TemperatureController::getFrSetTemp(){
	return m_frSetTemp;
}

void TemperatureController::setFzSetTemp(double temp){
	m_fzSetTemp = temp;
}

double TemperatureController::getFzSetTemp(){
	return m_fzSetTemp;
}

void TemperatureController::setCompressorDelayTime(unsigned long millis){
	m_compressorDelayTime = millis;
}

void TemperatureController::setDifference(double degrees){
	m_diff = degrees;
}

void TemperatureController::maintainTemperature(){
	double currentFrTemp = m_frSensor.readTemperature();
	double currentFzTemp = m_fzSensor.readTemperature();

	if (currentFrTemp > m_frSetTemp + m_diff) {
		// fridge to hot
		m_heaterRelay.off();
		m_baffel.open();
		m_fanRelay.on();
	} else if (currentFrTemp < m_frSetTemp - m_diff){
		// fridge to cold
		m_heaterRelay.on();
		m_baffel.close();
		m_fanRelay.off();
	} else {
		// fridge temp in ok range
		m_heaterRelay.off();
		m_baffel.close();
		if (!m_compressorRelay.isOn()){
			m_fanRelay.off();
		}
	}

	if (currentFzTemp > m_fzSetTemp + m_diff){
		// freezer to hot
		// wait m_diff after turning off compressor to turn it back on
		unsigned long currentMillis = millis();
		if ((unsigned long)(currentMillis - m_compressorTurnedOff) >= m_compressorDelayTime) {
			m_compressorRelay.on();
			m_fanRelay.on();
		}
	} else if (currentFzTemp < m_fzSetTemp - m_diff){
		// freezer to cold
		unsigned long m_compressorTurnedOff = millis();
		m_compressorRelay.off();
		if(!m_baffel.isOpen()){
			m_fanRelay.off();
		}
		// We don't worry about heating the freezer section as it doesn't really
		// matter if its a bit too cold and we save power.
	} else {
		// freezer temp in ok range
		unsigned long m_compressorTurnedOff = millis();
		m_compressorRelay.off();
		if(!m_baffel.isOpen()){
			m_fanRelay.off();
		}
	}
}
