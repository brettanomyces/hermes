#include <Baffel.h>
#include <Delay.h>
#include <DeviceManager.h>
#include <DoEvery.h>
#include <Relay.h>
#include <TemperatureController.h>
#include <TemperatureSensor.h>

// photon pins

int DATA_PIN = D0;  // DS/SER
int LATCH_PIN = D1;  // ST_CP/RCLK
int CLOCK_PIN = D2;  // Sh_CP/SRCLK

int FREEZER_SENSOR = A0;
int FRIDGE_SENSOR = A1;

// shift register pins

int IN1 = 0;
int IN2 = 1;
int IN3 = 2;
int IN4 = 3;

// stepper motor
// L1 = yellow
// L2 = red
// L3 = white 
// L4 = blue

int COMP_PIN = 4;
int FAN_PIN = 5;
int HEATER_PIN = 6;

// other constants

int UPDATE_PERIOD = 10000;  // 10 seconds
double COMP_DELAY = 300000;  // 5 minutes
double HEATER_DELAY = 30000;  // 30 seconds

double DEFAULT_FR_TEMP = 19.0;
double DEFAULT_FZ_TEMP = 4.0;

double ADC_STEPS = 4096;
double V_IN = 3.3;

int STEPPER_SPEED = 5;  // NOTE: also affected by shift register delay
int STEPPER_STEPS = 475;  // found via trial and error

int RELAY_ON = LOW;


DeviceManager deviceManager(DATA_PIN, LATCH_PIN, CLOCK_PIN);
DoEvery updateTimer(UPDATE_PERIOD);

TemperatureSensor fridgeSensor(2, 2, 10000, V_IN, ADC_STEPS);
TemperatureSensor freezerSensor(3, 2, 10000, V_IN, ADC_STEPS);

Baffel baffel(IN1, IN2, IN3, IN4, STEPPER_STEPS, STEPPER_SPEED, &deviceManager);
Relay compressor(COMP_PIN, COMP_DELAY, &deviceManager);
Relay fan(FAN_PIN, 0, &deviceManager);
Relay heater(HEATER_PIN, HEATER_DELAY, &deviceManager);

TemperatureController controller(
    baffel,
    compressor,
    fan,
    heater,
    freezerSensor,
    fridgeSensor);

void setup() {
  Serial.begin(9600);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

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

void loop() {
  if (updateTimer.check()) {
    // update state
    controller.maintainTemperature();

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
    Serial.print(compressor.isOn());
    Serial.print(SEPERATOR);
    Serial.print(compressor.isWaiting());
    Serial.print(SEPERATOR);
    Serial.print(fan.isOn());
    Serial.print(SEPERATOR);
    Serial.print(heater.isOn());
    Serial.print(SEPERATOR);
    Serial.print(heater.isWaiting());
    Serial.println();
  }
}

