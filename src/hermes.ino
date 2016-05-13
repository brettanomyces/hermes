#include "Arduino.h"
#include "Baffel.h"
#include "Delay.h"
#include "DeviceManager.h"
#include "DoEvery.h"
#include "Relay.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"

int FREEZER_SENSOR_PIN = 3;
int FRIDGE_SENSOR_PIN = 2;

// stepper motor
int IN1 = 12;  // = L1 = yellow
int IN2 = 11;  // = L2 = red
int IN3 = 10;  // = L3 = white 
int IN4 = 9;  // = L4 = blue
int EN_A = 13;  // always on
int EN_B = 8;  // always on

int COMP_PIN = 5;
int FAN_PIN = 6;
int HEATER_PIN = 7;

// other constants
int UPDATE_PERIOD = 10000;  // 10 seconds
double COMP_DELAY = 300000;  // 5 minutes
double FAN_DELAY = 0;
double HEATER_DELAY = 0;

double DEFAULT_FR_TEMP = 10.0;
double DEFAULT_FZ_TEMP = 4.0;

// temperature sensor
double ADC_STEPS = 1024;
double V_DIVIDER_V_IN = 5.0;
int V_DIVIDER_R1 = 10000;
int V_DIVIDER_THERMISTOR_POSITION = 2;

// baffel
int STEPPER_SPEED = 5;  // found via trial and error
int STEPPER_STEPS = 450;  // found via trial and error

int RELAY_ACTIVE_LOW = true;

DeviceManager deviceManager;
DoEvery updateTimer(UPDATE_PERIOD);

TemperatureSensor fridgeSensor(FRIDGE_SENSOR_PIN, V_DIVIDER_THERMISTOR_POSITION, V_DIVIDER_R1, V_DIVIDER_V_IN, ADC_STEPS);
TemperatureSensor freezerSensor(FREEZER_SENSOR_PIN, V_DIVIDER_THERMISTOR_POSITION, V_DIVIDER_R1, V_DIVIDER_V_IN, ADC_STEPS);

Baffel baffel(IN1, IN2, IN3, IN4, STEPPER_STEPS, STEPPER_SPEED, &deviceManager);
Relay compressor(COMP_PIN, COMP_DELAY, RELAY_ACTIVE_LOW, &deviceManager);
Relay fan(FAN_PIN, FAN_DELAY, RELAY_ACTIVE_LOW, &deviceManager);
Relay heater(HEATER_PIN, HEATER_DELAY, RELAY_ACTIVE_LOW, &deviceManager);

TemperatureController controller; 

void setup() {

  Serial.begin(9600);

  pinMode(FRIDGE_SENSOR_PIN, INPUT);
  pinMode(FREEZER_SENSOR_PIN, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_A, OUTPUT);
  digitalWrite(EN_A, HIGH);
  pinMode(EN_B, OUTPUT);
  digitalWrite(EN_B, HIGH);

  pinMode(COMP_PIN, OUTPUT);
  digitalWrite(COMP_PIN, HIGH);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, HIGH);
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, HIGH);

  // set state of components
  baffel.close();
  compressor.deactivate();
  fan.deactivate();
  heater.deactivate();

  // init timers
  updateTimer.reset();

  // set temps
  controller.setFzSetTemp(DEFAULT_FZ_TEMP);
  controller.setFrSetTemp(DEFAULT_FR_TEMP);
}

double frTemp = DEFAULT_FR_TEMP;
double fzTemp = DEFAULT_FZ_TEMP;

void loop() {
  if (updateTimer.check()) {
    frTemp = fridgeSensor.readTemperature();
    fzTemp = freezerSensor.readTemperature();

    if (compressor.isActive()) {
      if (controller.shouldDeactivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.deactivate();
        // compressor deactivated
      } 
    } else {  // compressor off
      if (controller.shouldActivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.activate();
        // compressor activated
      }
    }

    if (baffel.isOpen()) {
      if (controller.shouldCloseBaffel(frTemp)) {
        baffel.close();
        // baffel closed
      }
    } else { // baffel closed
      if (controller.shouldOpenBaffel(frTemp)) {
        baffel.open();
        // baffel opened
      }
    }

    if (heater.isActive()) {
      if (controller.shouldDeactivateHeater(fzTemp, heater.isWaiting())) {
        heater.deactivate();
        // heater deactivated
      } 
    } else {  // heater off
      if (controller.shouldActivateHeater(fzTemp, heater.isWaiting())) {
        heater.activate();
        // heater activated
      }
    }

    if (fan.isActive()) {
      if (controller.shouldDeactivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.deactivate();
        // fan deactivated
      } 
    } else {  // fan off
      if (controller.shouldActivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.activate();
        // fan activated
      }
    }
    
    // output values in the following csv format:
    // frs, fr, fzs, fz, b, c, cw, f, h, hw
    String SEPERATOR = ",";
    Serial.print(controller.getFrSetTemp());
    Serial.print(SEPERATOR);
    Serial.print(fridgeSensor.readTemperature());
    Serial.print(SEPERATOR);
    Serial.print(controller.getFzSetTemp());
    Serial.print(SEPERATOR);
    Serial.print(freezerSensor.readTemperature());
    Serial.print(SEPERATOR);
    Serial.print(baffel.isOpen());
    Serial.print(SEPERATOR);
    Serial.print(compressor.isActive());
    Serial.print(SEPERATOR);
    Serial.print(compressor.isWaiting());
    Serial.print(SEPERATOR);
    Serial.print(fan.isActive());
    Serial.print(SEPERATOR);
    Serial.print(heater.isActive());
    Serial.print(SEPERATOR);
    Serial.print(heater.isWaiting());
    Serial.println();
  }
}

