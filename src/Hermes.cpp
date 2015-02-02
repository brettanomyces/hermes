#include <Arduino.h>
#include "Baffel.h"
#include "Relay.h"
#include "TemperatureSensor.h"
#include "TemperatureController.h"

TemperatureSensor fridgeSensor(2, 2, 10000);
TemperatureSensor freezerSensor(3, 2, 10000);
Baffel baffel(13, 12, 11, 10, 9, 8, 4);
Relay compressor(5);
Relay fan(6);
Relay heater(7);
TemperatureController controller(
		baffel, 
		compressor, 
		fan, 
		heater,
		freezerSensor, 
		fridgeSensor
		);

void setup() {
	Serial.begin(9600);
	delay(5000);
	baffel.close();
} 

void loop() {
	controller.maintainTemperature();
	
	double fr = fridgeSensor.readTemperature();
	Serial.print("fr: ");
	Serial.print(fr);
	Serial.print(", ");

	double fz = freezerSensor.readTemperature();
	Serial.print("fz: "); 
	Serial.print(fz);
	Serial.print(", ");

	Serial.print("b: "); 
	Serial.print(baffel.isOpen());
	Serial.print(", ");

	Serial.print("c: "); 
	Serial.print(compressor.isOn());
	Serial.print(", ");

	Serial.print("f: "); 
	Serial.print(fan.isOn());
	Serial.print(", ");

	Serial.print("h: "); 
	Serial.print(heater.isOn());
	Serial.println();

	delay(5000);

}
