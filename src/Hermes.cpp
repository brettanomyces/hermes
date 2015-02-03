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

	Serial.print("frs: ");
	Serial.print(controller.getFrSetTemp());
	Serial.print(", ");

	Serial.print("fr: ");
	Serial.print(fridgeSensor.readTemperature());
	Serial.print(", ");

	Serial.print("fzs: "); 
	Serial.print(controller.getFzSetTemp());
	Serial.print(", ");

	Serial.print("fz: "); 
	Serial.print(freezerSensor.readTemperature());
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
