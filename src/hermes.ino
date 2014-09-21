/*
Hermes

I recently acquired a side by side fridge freezer. I plan to use the freezer
section of the FF as a keezer (keg fridge) and the fridge section as a
fermentation chamger. Unfortunately the temperature ranges allowed by the FF's
inbuilt digital temperature controller do not match up with the ranges I require
for my purpose.

The FF has a digital temperature controller (TC) which also controls the door
switches, lights and baffels.  Not wanting to rip out the exisiting TC I decided
to trick it into thinking the temperature is different to what it actually is.

The TC uses thermistors, resistors whose resistance changes with temperature, to
read the temperature. My unit (Hermes) basically sits inbetween the TC and the
thermistor, reading the resistance of the thermistor and offsetting it by some
about. The TC then reads this offset resistance. 

Hermes can take readings from up to 6 thermistors, however I will explain its
operation for only one thermistor per side of the FF.

#1 Read get the resistance of the thermistor. To do this the thermistor is
placed in a resistive divider, allowing the Arduino to read the voltage out and
calculate the resistance from that. 

#2 Convert the resistance to a temperature. This is done using the calculation
provided in the datasheet for the thermistors. 

#3 Offset this temperature. To do this I subtract the desired temperature (which
I have set) from the current temperature which I have measured. I then add the
current temperature setting from the TC. 

For example. I want the fridge at 20C, the current temperature is 18C and the TC
is set to 3C. I then set Hermes to (18 - 20) + 3 = 1C. The TC will try raise its
temperature by 2C, raising the actual temperature from 18C to 20C, my desired
temp.

#4 Set the resistance that the TC will read. To achieve I use two digital
potentiometers (pots), one 100kOhm and the other 5kOhm. Two pot are required
because the 100kOhm pot can only increment the resistance in 390 Ohm steps.
This is a problem becuase at high temperates the difference in resistance of a
thermistor between X degrees and X + 1 degrees is less than 390 Ohms. The 5kOhm
pot has a small enough step size to handle this but at low temperatures the
resistance of the thermistor will exceed 5kOhms. Course/fine grain adjustments,
scales. 


Pin Connections:

A0-A5 are each attached to the voltage out of a resistive divder. There can be
up to 6 thermistos, 3 for each side.

D13 is shared by both digital pots, connecting to their clock (CLK) pin via the
orange wire.

D11 is shared by both digital pots, connecting to their serial data in (SDI) pin
via the green wire.

D10 is connection to the 100kOhm pot's slave select (SS) pin via it's yellow wire.

D9 is connection to the 5kOhm pot's slave select (SS) pin via it's yellow wire.
*/

#include <SPI.h>
#include <math.h> 

// Each pot has 256 resistors -> 257 steps
const int STEPS = 257;
const int LARGE_SS = 9;
const int SMALL_SS = 10;
const double LARGE_OHMS = 100000;
const double SMALL_OHMS = 5000;

// OHMS / 256
const double LARGE_STEP_SIZE = 390.625;
const double SMALL_STEP_SIZE = 19.53125;

double LARGE_RW = 75;
double SMALL_RW = 75;

struct Section {
  // TODO improve naming
  // number is used to determine which inputs to read
  int number;
  // TODO improve naming
  // writeCmd determines which side of the pot to write to
  uint8_t writeCmd;
  int defaultTemp;
  int desiredTemp;
  int feedbackPin;
  int inputPin;
} fridge, freezer;

void setup() {
  
  // See table 7-2 in digital pot datasheet
  fridge.writeCmd = B00010000;
  fridge.defaultTemp = 3;
  fridge.desiredTemp = 20;
  fridge.feedbackPin = 5;
  fridge.inputPin = 3;

  // See table 7-2 in digital pot datasheet
  freezer.writeCmd = B00000000;
  freezer.defaultTemp = -18;
  freezer.desiredTemp = 7;
  freezer.feedbackPin = 0;
  freezer.inputPin = 2;

  Serial.begin(9600);

  // set the slaveSelectPin as an output
  pinMode(LARGE_SS, OUTPUT);
  pinMode(SMALL_SS, OUTPUT);

  // initialize SPI
  SPI.begin();

} // Void Setup Close

void loop() {
  
  // update temperature settings
  Serial.println("**** fridge ****");
  updateSection(fridge);
  Serial.println("**** freezer ****");
  updateSection(freezer);
  delay(10000);

}

void updateSection(struct Section s) {

  double thermistorRes = getInputResistance(s.inputPin);
  double currentTemp = resistanceToTemperature(thermistorRes);
  double offsetTemp = offsetTemperature(currentTemp, s.desiredTemp, s.defaultTemp);
  double offsetRes = temperatureToResistance(offsetTemp);
  
  int largePotValue = map(offsetRes, 0, LARGE_OHMS, 0, STEPS);
  // we want the large pot res to be less than the offset res so we don't set
  // some silly value for the small pot
  if (largePotValue * LARGE_STEP_SIZE > offsetRes ) {
    largePotValue--;
  }
  double largePotRes = largePotValue * LARGE_STEP_SIZE;
  // Invert value because I connectted to the A pin instead of the B.
  writeValue(LARGE_SS, s.writeCmd, uint8_t(STEPS - largePotValue));

  // Get the difference between the resistance we want and what we can set on
  // the large pot
  double deltaRes = offsetRes - largePotValue * LARGE_STEP_SIZE;

  int smallPotValue = map(deltaRes, 0, SMALL_OHMS, 0, STEPS);
  double smallPotRes = smallPotValue * SMALL_STEP_SIZE;
  double totalRes = largePotRes + smallPotRes;
  // Invert
  writeValue(SMALL_SS, s.writeCmd, uint8_t(STEPS - smallPotValue));

  double actualRes = getInputResistance(s.feedbackPin);

  Serial.print("temp: ");
  Serial.println(s.desiredTemp);

  Serial.print("curr res: ");
  Serial.println(thermistorRes);

  Serial.print("curr temp: ");
  Serial.println(currentTemp);

  Serial.print("offset temp: ");
  Serial.println(offsetTemp);

  Serial.print("offset res: ");
  Serial.println(offsetRes);

  Serial.print("actual res: ");
  Serial.println(actualRes);

  Serial.print("large pot: ");
  Serial.print(largePotValue);
  Serial.print(" -> ");
  Serial.println(largePotRes);

  Serial.print("diff: ");
  Serial.println(deltaRes);

  Serial.print("small pot: ");
  Serial.print(smallPotValue);
  Serial.print(" -> ");
  Serial.println(smallPotRes);

  Serial.print("total: ");
  Serial.println(totalRes);

  Serial.print("res err: ");
  Serial.println(offsetRes - totalRes);
}

double getInputResistance(int pin){
  double sum = 0;
  int i;
  for (i = 0; i < 100; i++){
    sum += (double) analogRead(pin);
  }
  double analog =  sum / 100.0;
  double inputVoltage  = analogToVoltage(analog);
  return voltageToResistance(inputVoltage);
}

double analogToVoltage(double analog){
  return (analog / 1024.0) * 5.0;
}

/* Calculate the resistance (r1) of the thermistor in the resistive divider
 * circuit 
 */
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

