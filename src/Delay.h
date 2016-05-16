#ifndef SRC_DELAY_H_
#define SRC_DELAY_H_
#include "IDeviceManager.h"

class Delay {
  public:
    explicit Delay(double period, IDeviceManager* deviceManager);
    void reset();
    bool ok();

  private:
    double m_period;
    double m_lastTime;
    IDeviceManager* m_dm;
};
#endif  // SRC_DELAY_H_
