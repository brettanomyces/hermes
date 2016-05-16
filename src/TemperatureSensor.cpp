#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(
    int pin,
    int thermistorPosition,
    int resistorValue,
    double vIn,
    double adcSteps,
    IDeviceManager* deviceManager) {
  m_pin = pin;
  m_thermistorPosition = thermistorPosition;
  m_resistorValue = resistorValue;
  m_vIn = vIn;
  m_adcSteps = adcSteps;
  m_dm = deviceManager;
}

double TemperatureSensor::readTemperature(){
  return analogToTemperature(m_dm->readAnalogValue(m_pin));
}

double TemperatureSensor::analogToTemperature(double analog) {
  double voltage  = analogToVoltage(analog);
  double resistance = voltageToResistance(voltage);
  double temperature = resistanceToTemperature(resistance);
  return temperature;;
}

double TemperatureSensor::analogToVoltage(double analog) {
  return (analog / static_cast<double>(m_adcSteps)) * m_vIn;
}

// Calculate the resistance of the thermistor in the resistive divider
// circuit. Note the calculation is different depending on whether the
// thermistor is in posiotion r1 or r2.

double TemperatureSensor::voltageToResistance(double vOut) {
  if (m_thermistorPosition == 1) {
    return calculateR1(vOut);
  } else {
    return calculateR2(vOut);
  }
}

double TemperatureSensor::calculateR1(double vOut) {
  double r2 = m_resistorValue;
  double r1 = ((r2 * m_vIn) / vOut) - r2;
  return r1;
}

double TemperatureSensor::calculateR2(double vOut) {
  double r1 = m_resistorValue;
  double r2 = r1 * (1 / ((m_vIn / vOut) - 1));
  return r2;
}

double TemperatureSensor::temperatureToResistance(double t) {
  // Resistance of thermistor at 25 degrees celcius
  int R25 = 5000;
  // Temperature in kelvin
  double tKelvin = t + 273.15;
  // Constants
  double A = -1.4141963E+01;
  double B = 4.4307830E+03;
  double C = -3.4078983E+04;
  double D = -8.8941929E+06;

  if (t >= -50 && t < 0) {
    A = -1.4122478E+01;
    B = 4.4136033E+03;
    C = -2.9034189E+04;
    D = -9.3875035E+06;
  } else if (t >= 0 && t < 50) {
    A = -1.4141963E+01;
    B = 4.4307830E+03;
    C = -3.4078983E+04;
    D = -8.8941929E+06;
  } else if (t >= 50 && t < 100) {
    A = -1.4202172E+01;
    B = 4.4975256E+03;
    C = -5.8421357E+04;
    D = -5.9658796E+06;
  } else if (t >= 100 && t < 150) {
    A = -1.6154078E+01;
    B = 6.8483992E+03;
    C = -1.0004049E+06;
    D = 1.1961431E+08;
  } else {
    // ERROR! Assume temp in 0C to 50C Range
  }
  return R25 * exp(A + B / tKelvin + C / pow(tKelvin, 2) + D / pow(tKelvin, 3));
}

double TemperatureSensor::resistanceToTemperature(double Rt) {
  // Resistance of thermistor at 25 degrees celcius
  double R25 = 5000;
  double Rt_over_R25 = Rt / R25;
  // Constants
  // Default values to 0 to 50 degree range
  double a = 3.3540154E-03;
  double b = 2.5627725E-04;
  double c = 2.0829210E-06;
  double d = 7.3003206E-08;

  if (Rt_over_R25 <= 68.600 && Rt_over_R25 > 3.274) {
    // -50 to 0 degrees
    a = 3.3538646E-03;
    b = 2.5654090E-04;
    c = 1.9243889E-06;
    d = 1.0969244E-07;
  } else if (Rt_over_R25  <= 3.274 && Rt_over_R25 > 0.36036) {
    // 0 to 50 degrees
    a = 3.3540154E-03;
    b = 2.5627725E-04;
    c = 2.0829210E-06;
    d = 7.3003206E-08;
  } else if (Rt_over_R25 <= 0.36036 && Rt_over_R25 > 0.06831) {
    // 50 to 100 degrees
    a = 3.3539264E-03;
    b = 2.5609446E-04;
    c = 1.9621987E-06;
    d = 4.6045930E-08;
  } else if (Rt_over_R25 <= 0.06831 && Rt_over_R25 > 0.01872) {
    // 100 to 150 degrees
    a = 3.3368620E-03;
    b = 2.4057263E-04;
    c = -2.6687093E-06;
    d = -4.0719355E-07;
  } else {
    // ERROR! Assume temp in 0C to 50C range
  }

  double log_Rt_over_R25 = log(Rt_over_R25);
  double tKelvin = pow(a + b * log_Rt_over_R25 + pow(c * log_Rt_over_R25, 2) + pow(d * log_Rt_over_R25, 3), -1);
  double tCelcius = tKelvin - 273.15;
  return tCelcius;
}
