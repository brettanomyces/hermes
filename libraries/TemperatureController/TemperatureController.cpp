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

/* 
 * Maintain the fridge and freezer set temperatures.
 *
 * Fridge is cooled by the baffel + fan, bringing cold air into the fridge section from the freezer section.
 * Fridge is warmed by a low wattage heater placed inside the fridge section
 * Freezer is cooled by the compressor + fan
 * Freezer is never warmed
 *
 * Example:
 * FreezerSetTemp: 5C
 * FridgeSetTemp: 10C
 * Diff: 0.5C
 * 
 * Baffel will open when FrTemp >= 10.5C
 * Baffel will close when FrTemp <= 9.5C
 * Note: Fan turns on whenever baffel is open, bringing cool air in from the Freezer section and cooling the fridge
 *
 * Heater will turn on when FrTemp <= 9.0C 
 * Heater will turn off at 10C
 * We assume that the temp tends to rise in each section so we turn heater off at the set temp rather than the set temp + diff
 * We assume that being above the set temp is worse than being below
 * We wait till 2 * Diff to turn on heater so we aren't immediately turning it on after closing the baffel. 
 *
 * Compressor will turn on when FzTemp >= 5.5C
 * Compressor will turn off then FzTemp <= 4.5C
 * Compressor must run for at least 5 minutes and must wait at least 5 minutes after being turned off
 * Fan turns on whenever the Compressor is on
 * No effort is made to warn Freezer section if gets too cold.
 *
 */
void TemperatureController::maintainTemperature() {
  double currentFrTemp = m_frSensor.readTemperature();
  double currentFzTemp = m_fzSensor.readTemperature();

  if (m_frEmpty) {
    // fridge is empty so don't try and maintain the temperature
    m_baffel.close();
    m_heater.deactivate();
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
    if (currentFrTemp < m_frSetTemp - 2 * m_diff) {
      // fridge too cold, turn on heater to raise temp
      m_heater.activate();
    } else if (currentFrTemp > m_frSetTemp) {
      // stop heating once fridge reaches the set temp
      m_heater.deactivate();
    }
  }

  // compressor
  if (currentFzTemp > m_fzSetTemp + m_diff) {
    // freezer too hot, turn on compressor to lower temp
    m_compressor.activate();
  } else if (currentFzTemp < m_fzSetTemp - m_diff) {
    // stop cooling once freezer reaches set temp
    m_compressor.deactivate();
  }

  // fan
  if (m_baffel.isOpen() || m_compressor.isOn()) {
    m_fan.activate();
  } else {
    m_fan.deactivate();
  }
}
