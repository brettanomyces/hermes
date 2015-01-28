#include <Arduino.h>
#include "TemperatureSensor.h"

int fridgeTempPin = 2;
int freezerTempPin = 3;

TemperatureSensor ts;

void setup() {
  Serial.begin(9600);
  delay(1000);
} 

void loop() {
  
  
  double fridgeTemp = ts.readTemperature(fridgeTempPin);
  double freezerTemp = ts.readTemperature(freezerTempPin);

  Serial.print("Fridge: ");
  Serial.print(fridgeTemp);
  Serial.println();

  Serial.print("Freezer: "); 
  Serial.print(freezerTemp);
  Serial.println();

  delay(1000);

}
