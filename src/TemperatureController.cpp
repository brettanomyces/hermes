#include "TemperatureController.h"

// Fridge is cooled by the baffel + fan, bringing cold air into the fridge section from the freezer section.
// Fridge is warmed by a low wattage heater placed inside the fridge section
// Freezer is cooled by the compressor + fan
// Freezer is never warmed
// 
// Example:
// FreezerSetTemp: 5C
// FridgeSetTemp: 10C
// Diff: 0.5C
// 
// Baffel will open when FrTemp >= 10.5C
// Baffel will close when FrTemp <= 9.5C
// Note: Fan turns on whenever baffel is open, bringing cool air in from the Freezer section and cooling the fridge
// 
// Heater will turn on when FrTemp <= 9.0C 
// Heater will turn off at 10C
// We assume that the temp tends to rise in each section so we turn heater off at the set temp rather than the set temp + diff
// We assume that being above the set temp is worse than being below
// We wait till 2 * Diff to turn on heater so we aren't immediately turning it on after closing the baffel. 
// 
// Compressor will turn on when FzTemp >= 5.5C
// Compressor will turn off then FzTemp <= 4.5C
// Compressor must run for at least 5 minutes and must wait at least 5 minutes after being turned off
// Fan turns on whenever the Compressor is on
// No effort is made to warn Freezer section if gets too cold.

TemperatureController::TemperatureController() {
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

double TemperatureController::getDifference() {
  return m_diff;
}

bool TemperatureController::shouldActivateCompressor(double fzTemp, bool isWaiting) {
  if (isWaiting) {
    return false;
  } else {
    if (fzTemp > m_fzSetTemp + m_diff) {
      return true;
    }
    return false;
  }
}

bool TemperatureController::shouldDeactivateCompressor(double fzTemp, bool isWaiting) {
  if (isWaiting) {
    return false;
  } else {
    if (fzTemp < m_fzSetTemp - m_diff) {
      return true;
    }
    return false;
  }
}

bool TemperatureController::shouldOpenBaffel(double frTemp) {
  if (frTemp > m_frSetTemp + m_diff) {
    return true;
  }
  return false;
}

bool TemperatureController::shouldCloseBaffel(double frTemp) {
  if (frTemp < m_frSetTemp - m_diff) {
    return true;
  }
  return false;
}

// NOTE: we are slower to turn on the heater (2 x diff) and faster to turn off
// (no diff) because the temp in the fridge will tend to rise by itself due to fermentation
// and it is usually worse to be too hot than too cold

bool TemperatureController::shouldActivateHeater(double frTemp, bool isWaiting) {
  if (isWaiting) {
    return false;
  } else {
    if (frTemp < m_frSetTemp - 2 * m_diff) {
      return true;
    }
    return false;
  }
}

bool TemperatureController::shouldDeactivateHeater(double frTemp, bool isWaiting) {
  if (isWaiting) {
    return false;
  } else {
    if (frTemp > m_frSetTemp) {
      return true;
    }
    return false;
  }
}

bool TemperatureController::shouldActivateFan(bool compressorOn, bool baffelOpen) {
  if (compressorOn || baffelOpen) {
    return true;
  }
  return false;
}

bool TemperatureController::shouldDeactivateFan(bool compressorOn, bool baffelOpen) {
  if (!compressorOn && !baffelOpen) {
    return true;
  }
  return false;
}
