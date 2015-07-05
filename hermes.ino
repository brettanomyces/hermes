#include <CmdMessenger.h>  // Arduino will look in 'libraries' folders for includes using <>
#include <DoEvery.h>
#include <Baffel.h>
#include <Stepper.h>
#include <Delay.h>
#include <Relay.h>
#include <TemperatureController.h>
#include <TemperatureSensor.h>

// how often do we check the temp
int UPDATE_PERIOD = 10000;  // 10 seconds

enum {
  kError,             // 0
  kAcknowledge,       // 1
  kSetFrSetTemp,      // 2
  kSetFzSetTemp,      // 3
  kForceOpenBaffel,   // 4
  kForceCloseBaffel,  // 5
  kSetFrEmpty         // 6
};

// Attach a new CmdMessen object ot the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

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

void attachCommandCallbacks() {
  cmdMessenger.attach(onUnknownCommand);
  cmdMessenger.attach(kSetFrSetTemp, setFrSetTemp);
  cmdMessenger.attach(kSetFzSetTemp, setFzSetTemp);
  cmdMessenger.attach(kForceOpenBaffel, forceOpenBaffel);
  cmdMessenger.attach(kForceCloseBaffel, forceCloseBaffel);
  cmdMessenger.attach(kSetFrEmpty, setFrEmpty);
}

void onUnknownCommand() {
  cmdMessenger.sendCmd(kError);
}

void setFrEmpty() {
  bool empty = cmdMessenger.readBoolArg();
  controller.setFrEmpty(empty);
}

void setFrSetTemp() {
  float temp = cmdMessenger.readFloatArg();
  controller.setFrSetTemp(temp);
}

void setFzSetTemp() {
  float temp = cmdMessenger.readFloatArg();
  controller.setFzSetTemp(temp);
}

void forceOpenBaffel() {
  baffel.forceOpen();
  cmdMessenger.sendCmd(kAcknowledge);
}

void forceCloseBaffel() {
  baffel.forceClose();
  cmdMessenger.sendCmd(kAcknowledge);
}

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

  // Print newline at the end of every command
  cmdMessenger.printLfCr();

  attachCommandCallbacks();

  // send the status to the pc that says the Arduino has booted
  cmdMessenger.sendCmd(kAcknowledge);
}

void loop() {
  cmdMessenger.feedinSerialData();

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
