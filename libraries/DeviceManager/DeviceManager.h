#ifndef SRC_DEVICEMANAGER_H_
#define SRC_DEVICEMANAGER_H_

class DeviceManager {
  public:
    DeviceManager(int data, int latch, int clock);

    void openBaffel();
    void closeBaffel();
    void activateRelay(int);
    void deactivateRelay(int);

  private:
    int m_data;
    int m_latch;
    int m_clock;

    int m_active = HIGH;

    void writeRegisters();
    void clearRegisters();
    void setRegisterPin(int, int);
};

#endif  // SRC_DEVICEMANAGER_H_


