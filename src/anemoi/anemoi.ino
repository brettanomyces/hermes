/*
Thermistor offset using digital pots

 * CS100 - to digital pin 10  (SS pin)
 * CS5 - to digital pin 9 (SS pin)
 * SDI - to digital pin 11 (MOSI pin)
 * CLK - to digital pin 13 (SCK pin)
 
 * A0 - Freezer thermistor #1 reading
 * A1 - Freezer thermistor #2 reading
 * A2 - Fridge thermistor #1 reading
 * A3 - Fridge thermistor #2 reading

*/

// include the SPI library
#include <SPI.h>
#include <math.h> 

// set pin 10 as the slave select for the difital pot
const int SLAVE_SELECT_100 = 10;
const int SLAVE_SELECT_5 = 9;
const int FREEZER_1 = 0;
const int FREEZER_2 = 1;
const int FRIDGE_1 = 2;
const int FRIDGE_2 = 3;

void setup() {

  Serial.begin(9600);
  // set the slaveSelectPin as an output
  pinMode(SLAVE_SELECT_100, OUTPUT);
  pinMode(SLAVE_SELECT_5, OUTPUT);

  // initialize SPI
  SPI.begin();

} // Void Setup Close

void loop() {

  float analog;

  int i;
  int j;
  for (i = 0; i < 3; i++){

    // Get average of ten readings 
    float sum = 0;
    for (j = 0; j < 10; j++) {
      sum +=  analogRead(i);
    }
    analog = sum / 10;

    Serial.print(calculateResistance(calculateVoltage(analog)));
    Serial.print(", ");
  }

  Serial.println();

  //Delay to make serial out readable
  delay(1000);

} // void loop close

float calculateVoltage(float analog){
  return (analog / 1024) * 5.0;
}

float calculateResistance(float voltage){
  return 100000 * (1 / ((5 / voltage) - 1));
}


double temperatureToResistance(double t) {
  int R25 = 5000;
  double tKelvin = t + 273.15;
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
  double R25 = 5000;
  double Rt_over_R25 = Rt / R25;
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
  double tKelvin = pow(a + b * log_Rt_over_R25 + pow(c * log_Rt_over_R25, 2) + pow(d * log_Rt_over_R25, 3), -1)
  double temp = tKelvin - 273.15;
  return temp;

}

void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip
  digitalWrite(slaveSelectPin, LOW);
  // send in the address and value via SPI
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip
  digitalWrite(slaveSelectPin, HIGH);
}

