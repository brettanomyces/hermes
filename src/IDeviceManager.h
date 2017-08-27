#ifndef SRC_IDEVICEMANAGER_H_
#define SRC_IDEVICEMANAGER_H_

class IDeviceManager {
  public:
    virtual void activateRelay(int, bool) = 0;
    virtual void deactivateRelay(int, bool) = 0;
    virtual unsigned long getMillis() = 0;
};
#endif  // SRC_IDEVICEMANAGER_H_
