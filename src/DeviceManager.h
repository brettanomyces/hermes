#ifndef SRC_DEVICEMANAGER_H_
#define SRC_DEVICEMANAGER_H_

#include "Arduino.h"
#include "IDeviceManager.h"

class DeviceManager : public IDeviceManager {
  public:
    DeviceManager();

    void openBaffel(int, int, int, int, int, int);
    void closeBaffel(int, int, int, int, int, int);
    void activateRelay(int, bool);
    void deactivateRelay(int, bool);
    double readAnalogValue(int);
    unsigned long getMillis();

  private:
    const int NUM_REPITIONS = 1000;

    void clockwise(int, int, int, int, int);
    void antiClockwise(int, int, int, int, int);
};

#endif  // SRC_DEVICEMANAGER_H_


