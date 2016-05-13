#ifndef SRC_DEVICEMANAGER_H_
#define SRC_DEVICEMANAGER_H_

#include "Arduino.h"

class DeviceManager {
  public:
    DeviceManager();

    void openBaffel(int, int, int, int, int, int);
    void closeBaffel(int, int, int, int, int, int);
    void activateRelay(int, bool);
    void deactivateRelay(int, bool);

  private:
    int m_active = HIGH;

    void clockwise(int, int, int, int, int);
    void antiClockwise(int, int, int, int, int);
};

#endif  // SRC_DEVICEMANAGER_H_


