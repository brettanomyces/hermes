#ifndef SRC_DEVICEMANAGER_H_
#define SRC_DEVICEMANAGER_H_

#include "Arduino.h"
#include "IDeviceManager.h"

class DeviceManager : public IDeviceManager {
  public:
    DeviceManager();

    void activateRelay(int, bool);
    void deactivateRelay(int, bool);
    unsigned long getMillis();
};

#endif  // SRC_DEVICEMANAGER_H_


