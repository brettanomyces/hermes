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
		m_frSensor(frSensor),
		m_delay(180000) // 3 minutes
		{
	// default values
	m_frSetTemp = 20.0;
	m_fzSetTemp = 10.0;
	m_diff = 0.5;
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

void TemperatureController::setDifference(double degrees){
	m_diff = degrees;
}

void TemperatureController::maintainTemperature(){
	double currentFrTemp = m_frSensor.readTemperature();
	double currentFzTemp = m_fzSensor.readTemperature();

	if (currentFrTemp > m_frSetTemp + m_diff) {
		Serial.println("fridge to hot");
		m_heaterRelay.off();
		m_baffel.open();
		m_fanRelay.on();
	} else if (currentFrTemp < m_frSetTemp - m_diff){
		Serial.println("fridge to cold");
		m_heaterRelay.on();
		m_baffel.close();
		m_fanRelay.off();
	} else {
		Serial.println("fridge ok");
		m_heaterRelay.off();
		m_baffel.close();
		// if the compressor is not on then we can turn off the fan as well
		if (!m_compressorRelay.isOn()){
			m_fanRelay.off();
		}
	}
	if (currentFzTemp > m_fzSetTemp + m_diff){
		Serial.println("freezer to hot");
		if (m_delay.check()) {
			m_compressorRelay.on();
			// fan should always be on when compressor is on
			m_fanRelay.on();
		} else {
			Serial.println("waiting to turn on comp");
		}
	} else if (currentFzTemp < m_fzSetTemp - m_diff){
		Serial.println("freezer to cold");
		if(m_delay.check()){
			m_compressorRelay.off();
		} else {
			Serial.println("waiting to turn off comp");
		}
		// if the baffel is open it means the fridge is still too warm so
		// keep the fan on
		if(!m_baffel.isOpen()){
			m_fanRelay.off();
		}
		// don't worry about heating the freezer section as it doesn't really
		// matter if its a bit too cold and we save power.
	} else {
		Serial.println("freezer ok");
		if(m_delay.check()){
			m_compressorRelay.off();
		} else {
			Serial.println("waiting to turn off comp");
		}
		if(!m_baffel.isOpen()){
			m_fanRelay.off();
		}
	}
}
