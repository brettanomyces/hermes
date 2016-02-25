#ifndef SRC_BAFFEL_H_
#define SRC_BAFFEL_H_

#include <DeviceManager.h>

class Baffel {
  public:
    Baffel(int in1, int in2, int in3, int in4, int steps, int speed, DeviceManager* deviceManager);

    void open();
    void close();
    bool isOpen();

  private:
    bool m_open;
    int m_in1;
    int m_in2;
    int m_in3;
    int m_in4;
    int m_steps;
    int m_speed;
    // reference to the device manager
    DeviceManager* m_dm;
};
#endif  // SRC_BAFFEL_H_
