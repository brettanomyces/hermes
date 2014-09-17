/*
 * Thermistor offset using digital pots
 * CS100 - YELLOW - to digital pin 10  (SS pin)
 * CS5 - YELLOW - to digital pin 9 (SS pin)
 * SDI - GREEN - to digital pin 11 (MOSI pin)
 * SDK (CLK) ORANGE - to digital pin 13 (SCK pin)
 *
 * There are 6 possible inputs and two outputs. 
 * The inputs are resistance values from a thermistor.
 * The ouputs are resistance values set on two digital pots in series.
 */

// include the SPI library
#include <SPI.h>
#include <math.h> 

// 256 resistors means 257 steps
const int STEPS = 257;
// slave select for the 100kOhm digital pot
const int LARGE_SS = 9;
const int LARGE_OHMS = 100000;
const double LARGE_STEP_SIZE = 390.625;
// slave select for the 5kOhm digital pot
const int SMALL_SS = 10;
const int SMALL_OHMS = 5000;
const double SMALL_STEP_SIZE = 19.53125;

struct Side {
  int number;
  uint8_t writeCmd;
  int defaultTemp;
  int desiredTemp;
} fridge, freezer;

void setup() {
  
  fridge.number = 0;
  fridge.writeCmd = B00000000;
  fridge.defaultTemp = 3;
  fridge.desiredTemp = 20;

  freezer.number = 1;
  freezer.writeCmd = B00010000;
  freezer.defaultTemp = -18;
  freezer.desiredTemp = 7;

  Serial.begin(9600);
  // set the slaveSelectPin as an output
  pinMode(LARGE_SS, OUTPUT);
  pinMode(SMALL_SS, OUTPUT);

  // initialize SPI
  SPI.begin();

} // Void Setup Close

void loop() {
  
  // update temperature settings
  updateSide(fridge);
  updateSide(freezer);
  delay(10000);

}

void updateSide(struct Side s) {
  double tr = getThermistorReading(s);
  double t = resistanceToTemperature(tr);
  double ot = offsetTemperature(t, s.desiredTemp, s.defaultTemp);
  double r = temperatureToResistance(ot);
  
  int lv = map(r, 0, LARGE_OHMS, 0, STEPS);
  // Invert value because I connectted to the A pin instead of the B.
  lv = STEPS - lv;
  writeValue(LARGE_SS, s.writeCmd, uint8_t(lv));

  // Get the difference between the resistance we want and what we can set on the large pot
  double dr = r - lv * LARGE_STEP_SIZE;

  int sv = map(dr, 0, SMALL_OHMS, 0, STEPS);
  // Invert
  sv = STEPS - sv;
  writeValue(SMALL_SS, s.writeCmd, uint8_t(sv));

  // TODO output to serial
}

double getThermistorReading(struct Side s){
  // themistors may not be plugged in so we need to keep track of how many readings we get.
  int n = 0;
  double sum = 0;
  // input values will be 0, 1, 2 for side 0 and 3, 4, 5 for side 1
  int i;
  for(i = 0; i < 3; i++){
    int input = i + s.number * 3;
    double analog = readInput(input);
    if (analog > 1.0 ){
      // if not then the thermistor is not plugged in.
      double inputVoltage  = analogToVoltage(analog);
      double inputResistance = voltageToResistance(inputVoltage);
      n += 1;
      sum += inputResistance;
    }
  }
  return  sum / (double)n;
}

double readInput(int pin){
  int sum = 0;
  int i;
  for (i = 1; i < 100; i ++){
    sum += analogRead(pin);
  }
  return (double)sum / 100.0;
}

double analogToVoltage(double analog){
  return (analog / 1024.0) * 5.0;
}

/* Calculate the resistance (r1) of the thermistor in the resistive divider circuit */
double voltageToResistance(double vOut){
  
  double vIn = 5; // Volts
  double r2 = 100000; // Ohms
  double r1 = ((r2 * vIn) / vOut) - r2;

  return r1;
}

double temperatureToResistance(double t) {

  // Resistance of thermistor at 25 degrees celcius
  int R25 = 5000;

  // Temperature in kelvin
  double tKelvin = t + 273.15;

  // Constants
  double A;
  double B;
  double C;
  double D;

  if ( t >= -50 && t < 0 ){
    A = -1.4122478E+01;
    B = 4.4136033E+03;
    C = -2.9034189E+04;
    D = -9.3875035E+06;
  } else if ( t >= 0 && t < 50) {
    // T > 0
    A = -1.4141963E+01;
    B = 4.4307830E+03;
    C = -3.4078983E+04;
    D = -8.8941929E+06;
  } else if ( t >= 50 && t < 100 ) {
    A = -1.4202172E+01;
    B = 4.4975256E+03;
    C = -5.8421357E+04;
    D = -5.9658796E+06;
  } else if ( t >= 100 && t < 150) {
    A = -1.6154078E+01;
    B = 6.8483992E+03;
    C = -1.0004049E+06;
    D = 1.1961431E+08;
  } else {
    // ERROR
  }
  return R25 * exp( A + B / tKelvin + C / pow(tKelvin, 2) + D / pow(tKelvin, 3));
}

double resistanceToTemperature(double Rt){

  // Resistance of thermistor at 25 degrees celcius
  double R25 = 5000;
  double Rt_over_R25 = Rt / R25;

  // Constants
  double a;
  double b;
  double c;
  double d;

  if (Rt_over_R25 <= 68.600 && Rt_over_R25 > 3.274){
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
    // ERROR
  }

  double log_Rt_over_R25 = log(Rt_over_R25);
  double tKelvin = pow(a + b * log_Rt_over_R25 + pow(c * log_Rt_over_R25, 2) + pow(d * log_Rt_over_R25, 3), -1);
  double tCelcius = tKelvin - 273.15;
  return tCelcius;

}

/* Returns the temperature that we want the device to think it is */
double offsetTemperature(double currentTemp, double desiredTemp, double defaultTemp){
  return defaultTemp + (currentTemp - desiredTemp);
}

/**
 * potSS is the microchip to write to, either the 100kOhm or the 5kOhm.
 * address is the digital pot within the microchip to write to.
 * value is the value to set for that micochip and pot.
 **/
void writeValue(int ss, uint8_t c, uint8_t v) {
  // take the SS pin low to select the chip
  digitalWrite(ss, LOW);
  SPI.transfer(c);
  SPI.transfer(v);
  // take the SS pin high to de-select the chip
  digitalWrite(ss, HIGH);
}

