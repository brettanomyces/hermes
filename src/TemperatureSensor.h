#ifndef SRC_TEMPERATURESENSOR_H_
#define SRC_TEMPERATURESENSOR_H_

#include <Arduino.h>
#include <math.h>

class TemperatureSensor {
  public:
    TemperatureSensor(int pin, int thermistorPosition, int resistorValue);
    double readTemperature();

  private:
    // variables
    int m_pin;
    // position of thermistor within the resistive divider, i.e. R1 or R2
    int m_thermistorPosition;
    // resistance of the resistor (not the thermistor) in ohms
    int m_resistorValue;

    // constants
    // voltage in
    static const double V_IN = 5.0;
    // arduino has a 10bit analog to digital converter
    static const double ANALOG_STEPS = 1024;
    // number of time to read temp before getting an average
    static const int NUM_REPITIONS = 100;

    double measureVoltage(int pin);
    double analogToVoltage(double analog);
    double voltageToResistance(double vOut);
    double calculateR1(double vOut);
    double calculateR2(double vOut);
    double temperatureToResistance(double t);
    double resistanceToTemperature(double Rt);
};
#endif  // SRC_TEMPERATURESENSOR_H_
