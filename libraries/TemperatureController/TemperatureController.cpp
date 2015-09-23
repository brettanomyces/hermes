#include <TemperatureController.h>

TemperatureController::TemperatureController(
    Baffel& baffel,
    Relay& compressor,
    Relay& fan,
    Relay& heater,
    TemperatureSensor& fzSensor,
    TemperatureSensor& frSensor) :
    m_baffel(baffel),
    m_compressor(compressor),
    m_fan(fan),
    m_heater(heater),
    m_fzSensor(fzSensor),
    m_frSensor(frSensor) {
  // default values
  m_frSetTemp = 25.0;
  m_fzSetTemp = 4.0;
  m_diff = 0.25;
}

void TemperatureController::setFrEmpty(bool empty) {
  m_frEmpty = empty;
}

bool TemperatureController::getFrEmpty() {
  return m_frEmpty;
}

void TemperatureController::setFrSetTemp(double temp) {
  m_frSetTemp = temp;
}

double TemperatureController::getFrSetTemp() {
  return m_frSetTemp;
}

void TemperatureController::setFzSetTemp(double temp) {
  m_fzSetTemp = temp;
}

double TemperatureController::getFzSetTemp() {
  return m_fzSetTemp;
}

void TemperatureController::setDifference(double degrees) {
  m_diff = degrees;
}

void TemperatureController::maintainTemperature() {
  double currentFrTemp = m_frSensor.readTemperature();
  double currentFzTemp = m_fzSensor.readTemperature();

  if (m_frEmpty) {
    // fridge is empty so don't try and maintain the temperature
    m_baffel.close();
    m_heater.off();
  } else {
    // baffel
    if (currentFrTemp > m_frSetTemp + m_diff) {
      // fridge too hot, open baffel to allow in cool air from freezer
      m_baffel.open();
    } else if (currentFrTemp < m_frSetTemp - m_diff) {
      // stop cooling once fridge reaches the set temp
      m_baffel.close();
    }
    // heater
    if (currentFrTemp < m_frSetTemp - m_diff) {
      // fridge too cold, turn on heater to raise temp
      m_heater.on();
    } else if (currentFrTemp > m_frSetTemp) {
      // stop heating once fridge reaches the set temp
      m_heater.off();
    }
  }

  // compressor
  if (currentFzTemp > m_fzSetTemp + m_diff) {
    // freezer too hot, turn on compressor to lower temp
    m_compressor.on();
  } else if (currentFzTemp < m_fzSetTemp - m_diff) {
    // stop cooling once freezer reaches set temp
    m_compressor.off();
  }

  // fan
  if (m_baffel.isOpen() || m_compressor.isOn()) {
    m_fan.on();
  } else {
    m_fan.off();
  }
}
