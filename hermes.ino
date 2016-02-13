#include <DoEvery.h>
#include <Baffel.h>
#include <Stepper.h>
#include <Delay.h>
#include <Relay.h>
#include <TemperatureController.h>
#include <TemperatureSensor.h>

// how often do we check the temp
int UPDATE_PERIOD = 10000;  // 10 seconds
double FR_SET_TEMP = 19.0;
double FZ_SET_TEMP = 4.0;
double COMP_DELAY = 300000;  // 5 minutes
double FAN_DELAY = 30000;  // 30 seconds

// photon pins
int DATA_PIN = D0;  // DS/SER
int LATCH_PIN = D1;  // ST_CP/RCLK
int CLOCK_PIN = D2;  // Sh_CP/SRCLK
int FREEZER_SENSOR = A0;
int FRIDGE_SENSOR = A1;

// shift register pins
int STEPPER_IN1 = 0;  
int STEPPER_IN2 = 1;  
int STEPPER_IN3 = 2;  
int STEPPER_IN4 = 3;  
int FAN_RELAY = 4;
int COMPRESSOR_RELAY = 5;
int HEATER_RELAY = 6;

// stepper motor
// L1 = yellow
// L2 = red
// L3 = white 
// L4 = blue
int STEPPER_SPEED = 5;  // NOTE: also affected by shift register delay
int STEPPER_STEPS = 475;  // found via trial and error

int NUM_SHIFT_REGISTER = 2;
int NUM_SHIFT_REGISTER_PINS = NUM_SHIFT_REGISTER * 8;
boolean registers[NUM_SHIFT_REGISTER_PINS];

int RELAY_ON = LOW;


DoEvery updateTimer(UPDATE_PERIOD);
TemperatureSensor fridgeSensor(2, 2, 10000);
TemperatureSensor freezerSensor(3, 2, 10000);
Baffel baffel(13, 12, 11, 10, 9, 8, 4);
Relay compressor(5, "compressor", COMP_DELAY);
Relay fan(6, "fan", 0);
Relay heater(7, "heater", FAN_DELAY);  // 30 seconds
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

}

// set all register pins to LOW
void clearRegisters(){
  for(int i = NUM_SHIFT_REGISTER_PINS - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 

// NOTE: only call AFTER all values are set
void writeRegisters(){

  digitalWrite(LATCH_PIN, LOW);

  for(int i = NUM_SHIFT_REGISTER_PINS - 1; i >=  0; i--){
    digitalWrite(CLOCK_PIN, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(CLOCK_PIN, HIGH);

  }
  digitalWrite(LATCH_PIN, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}

void clockwise() {
  // 1
  setRegisterPin(STEPPER_IN1, HIGH);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN4, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 2
  setRegisterPin(STEPPER_IN1, LOW);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN4, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 3
  setRegisterPin(STEPPER_IN1, LOW);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN4, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 4
  setRegisterPin(STEPPER_IN1, HIGH);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN4, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
}

void antiClockwise() {
  // 1
  setRegisterPin(STEPPER_IN4, HIGH);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN1, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 2
  setRegisterPin(STEPPER_IN4, LOW);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN2, HIGH);
  setRegisterPin(STEPPER_IN1, LOW);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 3
  setRegisterPin(STEPPER_IN4, LOW);
  setRegisterPin(STEPPER_IN3, HIGH);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN1, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
  // 4
  setRegisterPin(STEPPER_IN4, HIGH);
  setRegisterPin(STEPPER_IN3, LOW);
  setRegisterPin(STEPPER_IN2, LOW);
  setRegisterPin(STEPPER_IN1, HIGH);
  writeRegisters(); 
  delay(STEPPER_SPEED);
}



void loop() {
  if (updateTimer.check()) {
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
    Serial.print(compressor.waiting());
    Serial.print(SEPERATOR);
    Serial.print(fan.isOn());
    Serial.print(SEPERATOR);
    Serial.print(heater.isOn());
    Serial.print(SEPERATOR);
    Serial.print(heater.waiting());
    Serial.println();
  }
}
