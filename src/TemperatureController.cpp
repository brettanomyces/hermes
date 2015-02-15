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
		m_compressorRelay.on();
		// fan should always be on when compressor is on
		if(m_compressorRelay.isOn()){
			// compressor may not turn on if waiting for delay
			m_fanRelay.on();
		}
	} else if (currentFzTemp < m_fzSetTemp - m_diff){
		Serial.println("freezer to cold");
		m_compressorRelay.off();
		// compressor may not turn off if waiting for delay
		// if the baffel is open it means the fridge is still too warm so
		// keep the fan on
		if(!m_compressorRelay.isOn() && !m_baffel.isOpen()){
			m_fanRelay.off();
		}
		// don't worry about heating the freezer section as it doesn't really
		// matter if its a bit too cold and we save power.
	} else {
		Serial.println("freezer ok");
		m_compressorRelay.off();
		if(!m_compressorRelay.isOn() && !m_baffel.isOpen()){
			m_fanRelay.off();
		}
	}
}
