/*

 http://forum.arduino.cc/index.php?topic=158803.0
 Digital Pot Control
  
  Based on the original sketch for AD5206....
  
  This example controls a Microchip digital potentiometer.
  The MCP42 has 2 potentiometer channels. Each channel's pins are labeled
  PAx - connect this to voltage
  PWx - this is the pot's wiper, which changes when you set it
  PBx - connect this to ground.
 
 The MCP42 is SPI-compatible,and to command it, you send two bytes: 
 
 The first byte is the Command Byte which has this format when
 the next byte is to be data: XX01XXpp
 note these bits ...............^^....  the 01 means the next byte is data
 (where pp = potentiometer selection, X= don't care)
 pp= 00 = dummy code, no pot selected
 pp= 01 = pot0
 pp= 10 = pot1
 pp= 11 = both pots
 
 Simplest case is to have X= 0 so the Command Byte will be:
 pp= 00: 00010000 = 16 
 pp= 01: 00010001 = 17
 pp= 10: 00010010 = 18
 pp= 11: 00010011 = 19
 
 The second byte is the resistance value for the channel (0 - 255).  
 
 The circuit:
  * All PA pins of MCP42 connected to +5V
  * All PB pins of MCP42 connected to ground
  * An LED and a 220-ohm resisor in series connected from each PW pin to ground
  * CS - to digital pin 10  (SS pin)
  * SI - to digital pin 11 (MOSI pin)
  * SCK - to digital pin 13 (SCK pin)
 
 created 10 Aug 2010 
 by Tom Igoe
 
 Thanks to Heather Dewey-Hagborg for the original tutorial, 2005
 
 Version for MCP42xx April 2013, Jim Brown
 
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

// slave select for the 100kOhm digital pot
const int SLAVE_SELECT_100 = 9;
// slave select for the 5kOhm digital pot
const int SLAVE_SELECT_5 = 10;
const int FREEZER_1 = 0;
const int FREEZER_2 = 1;
const int FREEZER_3 = 2;
const int FRIDGE_1 = 3;
const int FRIDGE_2 = 4;
const int FRIDGE_3 = 5;

// default settings of internal temperature controller
const int FREEZER_DEFAULT_TEMP = -18;
const int FREEZER_DESIRED_TEMP = 7;
const int FRIDGE_DEFAULT_TEMP= 3;
const int FRIDGE_DESIRED_TEMP = 19;

// Resistance of each step on digital pots. Pots have 256 steps
const double LARGE_STEP = 390.5;
const double SMALL_STEP = 20;

const int POT_0 = 17;
const int POT_1 = 18;

/* struct Pot { */
/*   int slaveSelect; */
/*   int maxResistence; */
/*   double numSteps; */
/*   double stepSize; */
/* } */

/* struct Section { */
/*   int defaultTemp; */
/*   int desiredTemp; */
/*   int sensorOne; */
/*   int sensorTwo; */
/*   int sensorThree; */

/* }; */

void setup() {

  Serial.begin(9600);
  // set the slaveSelectPin as an output
  pinMode(SLAVE_SELECT_100, OUTPUT);
  pinMode(SLAVE_SELECT_5, OUTPUT);

  // initialize SPI
  SPI.begin();

} // Void Setup Close

void loop() {

  /*
     1. Read analog in voltage.
     2. Convert to resistence
     3. Convert to temperature
     4. Negate difference between desired temp and fridge/freezer cont temp
     5. Convert new temp to resistence 
     6. Set resistence on digital pots
   */

  float analog;

  int j;

  // Get average of ten readings 
  float sum = 0;
  for (j = 0; j < 10; j++) {
    sum +=  analogRead(5);
  }
  analog = sum / 10;

  Serial.print("Input ");
  Serial.print(5);
  Serial.println();

  double v = calculateVoltage(analog);
  Serial.print(v);
  Serial.print("V");
  Serial.println();

  double r = calculateResistance(v);
  Serial.print(r);
  Serial.print(" Ohms");
  Serial.println();

  double t = resistanceToTemperature(r);
  Serial.print(t);
  Serial.print(" degrees");
  Serial.println();

  double t2 = getTrickTemp(t, FRIDGE_DESIRED_TEMP, FRIDGE_DEFAULT_TEMP);
  Serial.print(t2);
  Serial.print(" degrees");
  Serial.println();

  double r2 = temperatureToResistance(t2);
  Serial.print(r2);
  Serial.print(" Ohms");
  Serial.println();

  double largePotR = setLargePot(r2);
  Serial.print(largePotR);
  Serial.print(" Ohms");
  Serial.println();

  double diff = r2 - largePotR;
  double smallPotR = setSmallPot(diff);
  Serial.print(smallPotR);
  Serial.print(" Ohms");
  Serial.println();

  Serial.println();

  //Delay to make serial out readable
  delay(3000);

} // void loop close


float calculateVoltage(float analog){
  return (analog / 1024) * 5.0;
}

/* Calculate the resistance (r1) of the thermistor in the resistive divider circuit */
float calculateResistance(float vOut){
  
  double vIn = 5; // Volts
  double r2 = 100000; // Ohms
  double r1 = ((r2 * vIn) / vOut) - r2;

  return r1;
}

/* Set the large pot value to the step closest to the given resistance. 
Return the acutal resistance value */
double setLargePot(double r){

  int value = map(r, 0, 100000, 0, 255);
  digitalPotWrite(SLAVE_SELECT_100, POT_0, value);
  Serial.print("large pot value: ");
  Serial.println(value);
  return value * LARGE_STEP;
}

double setSmallPot(double r){
  int value = map(r, 0, 5000, 0, 255);
  digitalPotWrite(SLAVE_SELECT_5, POT_0, value);
  Serial.print("small pot value: ");
  Serial.println(value);
  return value * SMALL_STEP;
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
double getTrickTemp(double currentTemp, double desiredTemp, double defaultTemp){
  return defaultTemp + (currentTemp - desiredTemp);
}

/**
 * potSS is the microchip to write to, either the 100kOhm or the 5kOhm.
 * address is the digital pot within the microchip to write to.
 * value is the value to set for that micochip and pot.
 **/
void digitalPotWrite(int potSS, int address, int value) {
  // take the SS pin low to select the chip
  digitalWrite(potSS, LOW);
  // send in the address and value via SPI
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip
  digitalWrite(potSS, HIGH);
}

