#include <Baffel.h>
#include <Delay.h>
#include <DeviceManager.h>
#include <DoEvery.h>
#include <Relay.h>
#include <TemperatureController.h>
#include <TemperatureSensor.h>

int FREEZER_SENSOR_PIN = 3;
int FRIDGE_SENSOR_PIN = 2;

// stepper motor
int IN1 = 12;  // = L1 = yellow
int IN2 = 11;  // = L2 = red
int IN3 = 10;  // = L3 = white 
int IN4 = 9;  // = L4 = blue
int EN_A = 13;  // always on
int EN_BA = 8;  // always on

int COMP_PIN = 5;
int FAN_PIN = 6;
int HEATER_PIN = 7;

// other constants
int UPDATE_PERIOD = 10000;  // 10 seconds
double COMP_DELAY = 300000;  // 5 minutes
double HEATER_DELAY = 30000;  // 30 seconds

double DEFAULT_FR_TEMP = 19.0;
double DEFAULT_FZ_TEMP = 4.0;

// temperature sensor
double ADC_STEPS = 1024;
double V_DIVIDER_V_IN = 5.0;
int V_DIVIDER_R1 = 10000;
int V_DIVIDER_THERMISTOR_POSITION = 2;

// baffel
int STEPPER_SPEED = 5;  // NOTE: also affected by shift register delay
int STEPPER_STEPS = 475;  // found via trial and error

DeviceManager deviceManager(DATA_PIN, LATCH_PIN, CLOCK_PIN);
DoEvery updateTimer(UPDATE_PERIOD);

TemperatureSensor fridgeSensor(FRIDGE_SENSOR_PIN, V_DIVIDER_THERMISTOR_POSITION, V_DIVIDER_R1, V_DIVIDER_V_IN, ADC_STEPS);
TemperatureSensor freezerSensor(FREEZER_SENSOR_PIN, V_DIVIDER_THERMISTOR_POSITION, V_DIVIDER_R1, V_DIVIDER_V_IN, ADC_STEPS);

Baffel baffel(IN1, IN2, IN3, IN4, STEPPER_STEPS, STEPPER_SPEED, &deviceManager);
Relay compressor(COMP_PIN, COMP_DELAY, &deviceManager);
Relay fan(FAN_PIN, 0, &deviceManager);
Relay heater(HEATER_PIN, HEATER_DELAY, &deviceManager);

TemperatureController controller(); 

void setup() {
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
  pinMode(FAN_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);


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
      if (controller.deactivateCompressor(fzTemp) && !compressor.isWaiting()) {
        compressor.deactivate();
        Particle.publish("compressor", "deactivated");
      } 
    } else {  // compressor off
      if (controller.activateCompressor(fzTemp) && !compressor.isWaiting()) {
        compressor.activate();
        Particle.publish("compressor", "activated");
      }
    }

    if (baffel.isOpen()) {
      if (controller.closeBaffel(frTemp)) {
        baffel.close();
        Particle.publish("baffel", "closed");
      }
    } else { // baffel closed
      if (controller.openBaffel(frTemp)) {
        baffel.open();
        Particle.publish("baffel", "opened");
      }
    }

    if (heater.isActive()) {
      if (controller.deactivateHeater(fzTemp) && !heater.isWaiting()) {
        heater.deactivate();
        Particle.publish("heater", "deactivated");
      } 
    } else {  // heater off
      if (controller.activateHeater(fzTemp) && !heater.isWaiting()) {
        heater.activate();
        Particle.publish("heater", "activated");
      }
    }

    if (fan.isActive()) {
      if (controller.deactivateFan(fzTemp)) {
        fan.deactivate();
        Particle.publish("fan", "deactivated");
      } 
    } else {  // fan off
      if (controller.activateFan(fzTemp)) {
        fan.activate();
        Particle.publish("fan", "activated");
      }
    }
    
    // TODO output logs
  }
}

