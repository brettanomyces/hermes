#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include "Baffel.h"
#include "Delay.h"
#include "DeviceManager.h"
#include "DoEvery.h"
#include "Relay.h"
#include "TemperatureController.h"

char SSID[] = "vodafoneFAF5";
char PASSWORD[] = "CB9FG6FY95";

IPAddress IP(192, 168, 1, 201);  // static IP
IPAddress GATEWAY(192, 168, 1, 1);
IPAddress SUBNET(255, 255, 255, 0);
WiFiServer server(80);

int ONE_WIRE_SENSOR_PIN = 14;
DeviceAddress frSensor1Address = { 0x28, 0xFF, 0xA5, 0xA0, 0x68, 0x14, 0x04, 0x36 };  // sensor #1
DeviceAddress frSensor2Address = { 0x28, 0xFF, 0xB7, 0xA0, 0x68, 0x14, 0x04, 0xC9 };  // sensor #2
DeviceAddress fzSensorAddress  = { 0x28, 0xFF, 0x0C, 0x3A, 0x63, 0x14, 0x03, 0x4E };  // sensor #3

int IN1 = 5;
int IN2 = 17;
int IN3 = 16;
int IN4 = 4;

int FAN_PIN = 25;
int HEATER_PIN = 26;
int COMP_PIN = 27;

// other constants
int UPDATE_PERIOD = 10000;  // 10 seconds
double COMP_DELAY = 300000;  // 5 minutes
double FAN_DELAY = 0;
double HEATER_DELAY = 0;

double DEFAULT_FR_TEMP = 10.0;
double DEFAULT_FZ_TEMP = 4.0;

// baffel
int STEPPER_DELAY_MICROS = 10000;
int STEPPER_STEPS = 1800;

int RELAY_ACTIVE_LOW = true;


OneWire oneWire(ONE_WIRE_SENSOR_PIN);
DallasTemperature temperatureSensors(&oneWire);

DeviceManager deviceManager;
DoEvery updateTimer(UPDATE_PERIOD, &deviceManager);

Baffel baffel(IN1, IN2, IN3, IN4, STEPPER_STEPS, STEPPER_DELAY_MICROS);

Relay compressor(COMP_PIN, COMP_DELAY, RELAY_ACTIVE_LOW, &deviceManager);
Relay fan(FAN_PIN, FAN_DELAY, RELAY_ACTIVE_LOW, &deviceManager);
Relay heater(HEATER_PIN, HEATER_DELAY, RELAY_ACTIVE_LOW, &deviceManager);

TemperatureController controller;

double frSet = DEFAULT_FR_TEMP;
double frTemp = DEFAULT_FR_TEMP;
double fzSet = DEFAULT_FZ_TEMP;
double fzTemp = DEFAULT_FZ_TEMP;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
String message;

void setup() {
  Serial.begin(115200);

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

  // set default temps
  controller.setFzSetTemp(DEFAULT_FZ_TEMP);
  controller.setFrSetTemp(DEFAULT_FR_TEMP);
   
  temperatureSensors.begin();
  baffel.close();
  compressor.deactivate();
  fan.deactivate();
  heater.deactivate();

  WiFi.config(IP, GATEWAY, SUBNET);
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWORD);
  } else {
    WiFi.reconnect();
  }
}

void loop() {
  if (updateTimer.check()) {

    message = "";

    temperatureSensors.requestTemperatures();

    float temp = temperatureSensors.getTempC(frSensor1Address);
    if (temp < 0) {  // -127.00
      Serial.println("Failed to read fridge temperature");
    } else {
      frTemp = temp;
    }

    temp = temperatureSensors.getTempC(fzSensorAddress);
    if (temp < 0) {  // -127.00
      Serial.println("Failed to read freezer temperature");
    } else {
      fzTemp = temp;
    }

    if (compressor.isActive()) {
      if (controller.shouldDeactivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.deactivate();
      }
    } else {  // compressor off
      if (controller.shouldActivateCompressor(fzTemp, compressor.isWaiting())) {
        compressor.activate();
      }
    }

    if (baffel.isOpen()) {
      if (controller.shouldCloseBaffel(frTemp)) {
        baffel.close();
      }
    } else { // baffel closed
      if (controller.shouldOpenBaffel(frTemp)) {
        baffel.open();
      }
    }

    if (heater.isActive()) {
      if (controller.shouldDeactivateHeater(frTemp, heater.isWaiting())) {
        heater.deactivate();
      }
    } else {  // heater off
      if (controller.shouldActivateHeater(frTemp, heater.isWaiting())) {
        heater.activate();
      }
    }

    if (fan.isActive()) {
      if (controller.shouldDeactivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.deactivate();
      }
    } else {  // fan off
      if (controller.shouldActivateFan(compressor.isActive(), baffel.isOpen())) {
        fan.activate();
      }
    }

    root["fridge_temperature"] = frTemp;
    root["freezer_temperature"] = fzTemp;
    root["baffel_open"] = baffel.isOpen();
    root["fan_active"] = fan.isActive();
    root["heater_active"] = heater.isActive();
    root["compressor_active"] = compressor.isActive();

    root.printTo(message);
    Serial.println(message);
  }

  // attempt to connect to Wifi for 5 seconds
  if (WiFi.status() != WL_CONNECTED) {
    server.end();
    WiFi.reconnect();
    for(int i = 0; i < 10; i++) {
      delay(500);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi Connected");
        Serial.println(WiFi.localIP());
        server.begin();
        break;
      }
    }
  }

  // check for incoming clients
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client = server.available();
    if (client) {
      while (client.connected()) {
        if (client.available()) {
          client.printf(
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %u\r\n\r\n%s",
            message.length(),
            message.c_str()
          );
          client.flush();
        }
      }
    }
  }
}
