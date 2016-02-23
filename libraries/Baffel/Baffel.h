#ifndef SRC_BAFFEL_H_
#define SRC_BAFFEL_H_

#include <DeviceManager.h>

class Baffel {
  public:
    Baffel(DeviceManager deviceManager);

    void open();
    void close();
    bool isOpen();

  private:
    bool m_open;
    DeviceManager m_dm;
};
#endif  // SRC_BAFFEL_H_
