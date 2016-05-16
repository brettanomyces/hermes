#ifndef SRC_IDEVICEMANAGER_H_
#define SRC_IDEVICEMANAGER_H_

class IDeviceManager {
  public:
    virtual void openBaffel(int, int, int, int, int, int);
    virtual void closeBaffel(int, int, int, int, int, int);
    virtual void activateRelay(int, bool);
    virtual void deactivateRelay(int, bool);
    virtual double readAnalogValue(int);
    virtual unsigned long getMillis();
};
#endif  // SRC_IDEVICEMANAGER_H_
