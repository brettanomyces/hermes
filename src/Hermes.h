#ifndef SRC_HERMES_H_
#define SRC_HERMES_H_

#include <Arduino.h>
#include <CmdMessenger.h>
#include <DoEvery.h>
#include "Baffel.h"
#include "Delay.h"
#include "Relay.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"

// how often do we check the temp
int UPDATE_PERIOD = 10000;  // 10 seconds

enum {
  kError,             // 0
  kAcknowledge,       // 1
  kSetFrSetTemp,      // 2
  kSetFzSetTemp,      // 3
  kForceOpenBaffel,   // 4
  kForceCloseBaffel,  // 5
  kSetFrEmpty         // 6
};

void attachCommandCallbacks();
void onUnknownCommand();
void setFzSetTemp();
void setFrSetTemp();
void forceOpenBaffel();
void forceCloseBaffel();
void setFrEmpty();

#endif  // SRC_HERMES_H_
