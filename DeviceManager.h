#ifndef SRC_DEVICEMANAGER_H_
#define SRC_DEVICEMANAGER_H_

// TODO use vector instead?
#define NUM_SHIFT_REGISTERS 2
#define NUM_SHIFT_REGISTER_PINS NUM_SHIFT_REGISTERS * 8

#include "Particle.h"

class DeviceManager {
  public:
    DeviceManager(int data, int latch, int clock);

    void openBaffel(int, int, int, int, int, int);
    void closeBaffel(int, int, int, int, int, int);
    void activateRelay(int);
    void deactivateRelay(int);

  private:
    int m_data;
    int m_latch;
    int m_clock;
    boolean m_registers[NUM_SHIFT_REGISTER_PINS];

    int m_active = HIGH;

    void writeRegisters();
    void clearRegisters();
    void setRegisterPin(int, int);
    void clockwise(int, int, int, int);
    void antiClockwise(int, int, int, int);
};

#endif  // SRC_DEVICEMANAGER_H_


