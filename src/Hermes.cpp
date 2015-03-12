#include "Hermes.h"

// Attach a new CmdMessen object ot the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

DoEvery updateTimer(updateInterval);

TemperatureSensor fridgeSensor(2, 2, 10000);
TemperatureSensor freezerSensor(3, 2, 10000);
Baffel baffel(13, 12, 11, 10, 9, 8, 4);
Relay compressor(5, "compressor", 300000); // 5 minute delay
Relay fan(6, "fan", 0);
Relay heater(7, "heater", 30000); // 30 second delay
TemperatureController controller(
    baffel, 
    compressor, 
    fan, 
    heater,
    freezerSensor, 
    fridgeSensor
    );

void attachCommandCallbacks() {
  cmdMessenger.attach(onUnknownCommand);
  cmdMessenger.attach(kSetFrSetTemp, setFrSetTemp);
  cmdMessenger.attach(kSetFzSetTemp, setFzSetTemp);
  cmdMessenger.attach(kForceOpenBaffel, forceOpenBaffel);
  cmdMessenger.attach(kForceCloseBaffel, forceCloseBaffel);
}

void onUnknownCommand(){
  cmdMessenger.sendCmd(kError);
}

void setFrSetTemp(){
  float temp = cmdMessenger.readFloatArg();
  controller.setFrSetTemp(temp);
}

void setFzSetTemp(){
  float temp = cmdMessenger.readFloatArg();
  controller.setFzSetTemp(temp);
}

void forceOpenBaffel(){
  baffel.forceOpen();
  cmdMessenger.sendCmd(kAcknowledge);
}

void forceCloseBaffel(){
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

  if(updateTimer.check()) {
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
