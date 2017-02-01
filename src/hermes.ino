// #define PARTICLE

#ifdef PARTICLE
#include "Particle.h"
#else  // Arduino
#include "Arduino.h"
#endif

#include "Baffel.h"
#include "Delay.h"
#include "DeviceManager.h"
#include "DoEvery.h"
#include "Relay.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"

#ifdef PARTICLE
int FREEZER_SENSOR_PIN = 0;
int FRIDGE_SENSOR_PIN = 1;

// stepper motor
int IN1 = 0;  // = L1 = yellow
int IN2 = 1;  // = L2 = red
int IN3 = 2;  // = L3 = white
int IN4 = 3;  // = L4 = blue

int COMP_PIN = 4;
int FAN_PIN = 5;
int HEATER_PIN = 6;

// temperature sensor
double ADC_STEPS = 4096;
double V_DIVIDER_V_IN = 3.3;
#else  // Arduino
int FREEZER_SENSOR_PIN = 3;
int FRIDGE_SENSOR_PIN = 2;

int IN1 = 12;
int IN2 = 11;
int IN3 = 10;
int IN4 = 9;

int COMP_PIN = 5;
int FAN_PIN = 6;
int HEATER_PIN = 7;

double ADC_STEPS = 1024;
double V_DIVIDER_V_IN = 5.0;
#endif

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

#ifdef PARTICLE
// optional data for Particle.publish()
char data[64];
#endif

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
  #ifdef PARTICLE
  Particle.variable("frSet", frSet);
  Particle.variable("frTemp", frTemp);
  Particle.variable("fzSet", fzSet);
  Particle.variable("fzTemp", fzTemp);
  Particle.variable("compActive", compActive);
  Particle.variable("compWait", compWait);
  Particle.variable("baffelOpen", baffelOpen);
  Particle.variable("fanActive", fanActive);
  Particle.variable("heatActive", heatActive);
  Particle.variable("heatWait", heatWait);
  #endif

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

    #ifdef PARTICLE
    sprintf(data, "{\"frTemp\":\"%.2f\",\"fzTemp\":\"%.2f\"}", frTemp, fzTemp);
    Particle.publish("reading", data);
    #endif

    if (compressor.isActive()) {
      if (controller.shouldDeactivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.deactivate();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"compressor\",\"state\":\"off\"}");
        Particle.publish("event", data);
        #endif
      }
    } else {  // compressor off
      if (controller.shouldActivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.activate();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"compressor\",\"state\":\"on\"}");
        Particle.publish("event", data);
        #endif
      }
    }

    if (baffel.isOpen()) {
      if (controller.shouldCloseBaffel(frTemp)) {
        baffel.close();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"baffel\",\"state\":\"closed\"}");
        Particle.publish("event", data);
        #endif
      }
    } else { // baffel closed
      if (controller.shouldOpenBaffel(frTemp)) {
        baffel.open();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"baffel\",\"state\":\"open\"}");
        Particle.publish("event", data);
        #endif
      }
    }

    if (heater.isActive()) {
      if (controller.shouldDeactivateHeater(frTemp, heater.isWaiting())) {
        heater.deactivate();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"heater\",\"state\":\"off\"}");
        Particle.publish("event", data);
        #endif
      }
    } else {  // heater off
      if (controller.shouldActivateHeater(frTemp, heater.isWaiting())) {
        heater.activate();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"heater\",\"state\":\"on\"}");
        Particle.publish("event", data);
        #endif
      }
    }

    if (fan.isActive()) {
      if (controller.shouldDeactivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.deactivate();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"fan\",\"state\":\"off\"}");
        Particle.publish("event", data);
        #endif
      }
    } else {  // fan off
      if (controller.shouldActivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.activate();
        #ifdef PARTICLE
        sprintf(data, "{\"device\":\"fan\",\"state\":\"on\"}");
        Particle.publish("event", data);
        #endif
      }
    }

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

