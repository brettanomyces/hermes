#ifndef _HERMES_TEMP_SENSOR_
#define _HERMES_TEMP_SENSOR_

#include <Arduino.h>
#include <math.h>

class TemperatureSensor {

  public:
    // Constructor
    TemperatureSensor(int pin, int thermistorPosition, int resistorValue);

    double readTemperature();

  private:
    // Variables
    int m_pin;
    int m_thermistorPosition; // position of thermistor within the resistive divider, i.e. R1 or R2
    int m_resistorValue; // resistance of the resistor (not the thermistor) in ohms

    // Constants
    static const double V_IN = 5.0; // voltage in
    static const double ANALOG_STEPS = 1024; // arduino has a 10bit analog to digital converter
    static const int NUM_REPITIONS = 100; // number of time to read temp before getting an average
    
    double measureVoltage(int pin);
    double analogToVoltage(double analog);
    double voltageToResistance(double vOut);
    double calculateR1(double vOut);
    double calculateR2(double vOut);
    double temperatureToResistance(double t);
    double resistanceToTemperature(double Rt);
};

#endif // _HERMES_TEMP_SENSOR_
