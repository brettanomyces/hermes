#ifndef SRC_RELAY_H_
#define SRC_RELAY_H_

#include "Delay.h"
#include "DeviceManager.h"

class Relay {
  public:
    Relay(int pin, double delay, bool activeLow, DeviceManager* deviceManager);

    void activate();
    void deactivate();
    bool isActive();
    bool isWaiting();

  private:
    int m_pin;
    bool m_active;
    bool m_activeLow;
    Delay m_delay;
    DeviceManager* m_dm;
};

#endif  // SRC_RELAY_H_
