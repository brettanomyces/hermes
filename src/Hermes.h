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
  kError,           // 0
  kAcknowledge,     // 1
  kSetFrSetTemp,    // 2
  kSetFzSetTemp,    // 3
  kForceOpenBaffel, // 4
  kForceCloseBaffel // 5
};

void attachCommandCallbacks();
void onUnknownCommand();
void setFzSetTemp();
void setFrSetTemp();
void forceOpenBaffel();
void forceCloseBaffel();

#endif // _HERMES_
