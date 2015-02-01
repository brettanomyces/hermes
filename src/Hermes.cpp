#include <Arduino.h>
#include "TemperatureSensor.h"
#include "Baffel.h"

TemperatureSensor fridgeSensor(2, 2, 10000);
TemperatureSensor freezerSensor(3, 2, 10000);
Baffel baffel(13, 12, 11, 10, 9, 8);

void setup() {
  baffel.close();
  Serial.begin(9600);
  delay(1000);
} 

void loop() {
  
  double fridgeTemp = fridgeSensor.readTemperature();
  double freezerTemp = freezerSensor.readTemperature();

  Serial.print("Fridge: ");
  Serial.print(fridgeTemp);
  Serial.println();

  Serial.print("Freezer: "); 
  Serial.print(freezerTemp);
  Serial.println();

  delay(1000);

}
