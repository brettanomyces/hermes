// include the SPI library
#include <SPI.h>

// set pin 10 as the slave select for the difital pot
const int slaveSelectPin = 10;
const float OFFSET = 5000;

void setup() {

  Serial.begin(9600);
  // set the slaveSelectPin as an output
  pinMode(slaveSelectPin, OUTPUT);
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

int resistanceToValue(float resistance) {
  return map(resistence, 0, 100000, 0, 257);
}

float offsetResistance(float resistance) {
  return resistance - OFFSET;
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
