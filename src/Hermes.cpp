#include "Hermes.h"

// Attach a new CmdMessen object ot the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

DoEvery updateTimer(updateInterval);

TemperatureSensor fridgeSensor(2, 2, 10000);
TemperatureSensor freezerSensor(3, 2, 10000);
Baffel baffel(13, 12, 11, 10, 9, 8, 4);
Relay compressor(5, 180000); // 3 minute delay
Relay fan(6);
Relay heater(7, 30000); // 30 second delay
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
  cmdMessenger.attach(kOpenBaffel, openBaffel);
}

void onUnknownCommand(){
  cmdMessenger.sendCmd(kError);
}

void onArduinoReady(){
  cmdMessenger.sendCmd(kAcknowledge, "Arduino ready");
}

void setFrSetTemp(){
  float temp = cmdMessenger.readFloatArg();
  controller.setFrSetTemp(temp);
}

void setFzSetTemp(){
  float temp = cmdMessenger.readFloatArg();
  controller.setFzSetTemp(temp);
}

void openBaffel(){
  bool open =  cmdMessenger.readBoolArg();
  if (open) {
    baffel.open();
  } else {
    baffel.close();
  }
}

void setup() {
  // Listen on serial connection for messages from the pc
  Serial.begin(9600);

  // set state of components
  baffel.close();
  compressor.off();
  fan.off();
  heater.off();

  // init timers
  updateTimer.reset();

  // Print newline at the end of every command
  cmdMessenger.printLfCr();

  attachCommandCallbacks();

  // send the status to the pc that says the Arduino has booted
  cmdMessenger.sendCmd(kAcknowledge, "Arduino has started");
} 

void loop() {
  cmdMessenger.feedinSerialData();

  if(updateTimer.check()) {
    controller.maintainTemperature(); 
    //cmdMessenger.sendCmdStart(kPlotDataPoint);
    //cmdMessenger.sendCmdArg(fridgeSensor.readTemperature());
    //cmdMessenger.sendCmdArg(freezerSensor.readTemperature());
    //cmdMessenger.sendCmdArg(baffel.isOpen());
    //cmdMessenger.sendCmdArg(compressor.isOn());
    //cmdMessenger.sendCmdArg(fan.isOn());
    //cmdMessenger.sendCmdArg(heater.isOn());
    //cmdMessenger.sendCmdEnd();
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
