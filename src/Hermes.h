#ifndef _HERMES_
#define _HERMES_

#include <Arduino.h>
#include <CmdMessenger.h>
#include <DoEvery.h>
#include "Baffel.h"
#include "Delay.h"
#include "Relay.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"

int updateInterval = 10000;

enum {
  kAcknowledge,
  kError,
  kSetFrSetTemp,
  kSetFzSetTemp,
  kOpenBaffel,
  kPlotDataPoint,
  kLogFrTemp,
  kLogFzTemp,
  kLogBaffelState,
  kLogCompressorState,
  kLogFanState,
  kLogHeaterState,
};

void attachCommandCallbacks();
void onUnknownCommand();
void onArduinoReady();
void openBaffel();
void setFzSetTemp();
void setFrSetTemp();

#endif // _HERMES_
