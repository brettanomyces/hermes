#include "Arduino.h"
#include "Baffel.h"
#include "Delay.h"
#include "DeviceManager.h"
#include "DoEvery.h"
#include "Relay.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"

int FREEZER_SENSOR_PIN = 36;
int FRIDGE_SENSOR_PIN = 39;

int IN1 = 34;
int IN2 = 35;
int IN3 = 32;
int IN4 = 33;

int FAN_PIN = 25;
int HEATER_PIN = 26;
int COMP_PIN = 27;

double ADC_STEPS = 4096;
double V_DIVIDER_V_IN = 3.3;

// other constants
int UPDATE_PERIOD = 10000;  // 10 seconds
double COMP_DELAY = 300000;  // 5 minutes
double FAN_DELAY = 0;
double HEATER_DELAY = 0;

double DEFAULT_FR_TEMP = 19.0;
double DEFAULT_FZ_TEMP = 4.0;

int V_DIVIDER_R1 = 10000;
int V_DIVIDER_THERMISTOR_POSITION = 2;

// baffel
int STEPPER_SPEED = 5;  // found via trial and error
int STEPPER_STEPS = 450;  // found via trial and error

int RELAY_ACTIVE_LOW = true;

DeviceManager deviceManager;
DoEvery updateTimer(UPDATE_PERIOD, &deviceManager);

TemperatureSensor fridgeSensor(
  FRIDGE_SENSOR_PIN,
  V_DIVIDER_THERMISTOR_POSITION,
  V_DIVIDER_R1,
  V_DIVIDER_V_IN,
  ADC_STEPS,
  &deviceManager);

TemperatureSensor freezerSensor(
  FREEZER_SENSOR_PIN,
  V_DIVIDER_THERMISTOR_POSITION,
  V_DIVIDER_R1,
  V_DIVIDER_V_IN,
  ADC_STEPS,
  &deviceManager);

Baffel baffel(IN1, IN2, IN3, IN4, STEPPER_STEPS, STEPPER_SPEED, &deviceManager);
Relay compressor(COMP_PIN, COMP_DELAY, RELAY_ACTIVE_LOW, &deviceManager);
Relay fan(FAN_PIN, FAN_DELAY, RELAY_ACTIVE_LOW, &deviceManager);
Relay heater(HEATER_PIN, HEATER_DELAY, RELAY_ACTIVE_LOW, &deviceManager);

TemperatureController controller;

bool deviceStateSet = false;

double frSet = DEFAULT_FR_TEMP;
double frTemp = DEFAULT_FR_TEMP;
double fzSet = DEFAULT_FZ_TEMP;
double fzTemp = DEFAULT_FZ_TEMP;
bool compActive = false;
bool compWait = false;
bool baffelOpen = false;
bool fanActive = false;
bool heatActive = false;
bool heatWait = false;

void setup() {
  Serial.begin(115200);

  pinMode(FRIDGE_SENSOR_PIN, INPUT);
  pinMode(FREEZER_SENSOR_PIN, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(COMP_PIN, OUTPUT);
  digitalWrite(COMP_PIN, HIGH);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, HIGH);
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, HIGH);

  // init timers
  updateTimer.reset();

  // set temps
  controller.setFzSetTemp(DEFAULT_FZ_TEMP);
  controller.setFrSetTemp(DEFAULT_FR_TEMP);
}

void loop() {
  if(!deviceStateSet){
    baffel.close();
    compressor.deactivate();
    fan.deactivate();
    heater.deactivate();
    deviceStateSet = true;
  }

  if (updateTimer.check()) {
    frTemp = fridgeSensor.readTemperature();
    fzTemp = freezerSensor.readTemperature();

    // Arduino cannot sprintf floats
    Serial.print("{\"frTemp\":");
    Serial.print(frTemp);
    Serial.print("\",\"fzTemp\":");
    Serial.print(fzTemp);
    Serial.println("\"}");

    String message;
    if (compressor.isActive()) {
      if (controller.shouldDeactivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.deactivate();
        message = "{\"device\":\"compressor\",\"state\":\"off\"}";
      }
    } else {  // compressor off
      if (controller.shouldActivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.activate();
        message = "{\"device\":\"compressor\",\"state\":\"on\"}";
      }
    }
    Serial.println(message);

    if (baffel.isOpen()) {
      if (controller.shouldCloseBaffel(frTemp)) {
        baffel.close();
        message = "{\"device\":\"baffel\",\"state\":\"closed\"}";
      }
    } else { // baffel closed
      if (controller.shouldOpenBaffel(frTemp)) {
        baffel.open();
        message = "{\"device\":\"baffel\",\"state\":\"open\"}";
      }
    }
    Serial.println(message);

    if (heater.isActive()) {
      if (controller.shouldDeactivateHeater(frTemp, heater.isWaiting())) {
        heater.deactivate();
        message = "{\"device\":\"heater\",\"state\":\"off\"}";
      }
    } else {  // heater off
      if (controller.shouldActivateHeater(frTemp, heater.isWaiting())) {
        heater.activate();
        message = "{\"device\":\"heater\",\"state\":\"on\"}";
      }
    }
    Serial.println(message);

    if (fan.isActive()) {
      if (controller.shouldDeactivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.deactivate();
        message = "{\"device\":\"fan\",\"state\":\"off\"}";
      }
    } else {  // fan off
      if (controller.shouldActivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.activate();
        message = "{\"device\":\"fan\",\"state\":\"on\"}";
      }
    }
    Serial.println(message);

    // update variable
    frSet = controller.getFrSetTemp();
    frTemp = fridgeSensor.readTemperature();
    fzSet = controller.getFzSetTemp();
    fzTemp = freezerSensor.readTemperature();
    baffelOpen = baffel.isOpen();
    compActive = compressor.isActive();
    compWait = compressor.isWaiting();
    fanActive = fan.isActive();
    heatActive = heater.isActive();
    heatWait = heater.isWaiting();
  }
}
