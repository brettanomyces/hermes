#include <DoEvery.h>
#include <Baffel.h>
#include <Stepper.h>
#include <Delay.h>
#include <Relay.h>
#include <TemperatureController.h>
#include <TemperatureSensor.h>

// how often do we check the temp
int UPDATE_PERIOD = 10000;  // 10 seconds
double FR_SET_TEMP = 25.0;
double FZ_SET_TEMP = 4.0;

DoEvery updateTimer(UPDATE_PERIOD);
TemperatureSensor fridgeSensor(2, 2, 10000);
TemperatureSensor freezerSensor(3, 2, 10000);
Baffel baffel(13, 12, 11, 10, 9, 8, 4);
Relay compressor(5, "compressor", 300000);  // 5 minutes
Relay fan(6, "fan", 0);
Relay heater(7, "heater", 30000);  // 30 seconds
TemperatureController controller(
    baffel,
    compressor,
    fan,
    heater,
    freezerSensor,
    fridgeSensor);

void setup() {
  // Listen on serial connection for messages from the pc
  Serial.begin(9600);

  // set state of components
  baffel.forceClose();
  compressor.off();
  fan.off();
  heater.off();

  // init timers
  updateTimer.reset();

  // set temps
  controller.setFzSetTemp(FZ_SET_TEMP);
  controller.setFrSetTemp(FR_SET_TEMP);
}

void loop() {
  if (updateTimer.check()) {
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
    Serial.print("cw: ");
    Serial.print(compressor.waiting());
    Serial.print(", ");
    Serial.print("f: ");
    Serial.print(fan.isOn());
    Serial.print(", ");
    Serial.print("h: ");
    Serial.print(heater.isOn());
    Serial.print(", ");
    Serial.print("hw: ");
    Serial.print(heater.waiting());
    Serial.println();
  }
}
