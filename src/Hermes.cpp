#include <Arduino.h>
#include "Baffel.h"
#include "Relay.h"
#include "TemperatureSensor.h"


TemperatureSensor fridgeSensor(2, 2, 10000);
TemperatureSensor freezerSensor(3, 2, 10000);
Baffel baffel(13, 12, 11, 10, 9, 8);
Relay fan(4);
Relay compressor(5);
Relay heater(6);

void setup() {
  baffel.close();
  fan.off();
  compressor.off();
  heater.off();

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
