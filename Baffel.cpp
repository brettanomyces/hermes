#include <Baffel.h>

Baffel::Baffel(int in1, int in2, int in3, int in4, int steps, int speed, DeviceManager* deviceManager){
  m_in1 = in1;
  m_in2 = in2;
  m_in3 = in3;
  m_in4 = in4;
  m_steps = steps;
  m_speed = speed;
  m_dm = deviceManager;
  m_open = false;
}

// attempt to open baffel. does nothing if baffel is already open
void Baffel::open() {
  if (!isOpen()) {
    m_dm->openBaffel(m_in1, m_in2, m_in3, m_in4, m_steps, m_speed);
  }
}

// attempt to close baffel. does nothing if baffel is already closed
void Baffel::close() {
  if (isOpen()) {
    m_dm->closeBaffel(m_in1, m_in2, m_in3, m_in4, m_steps, m_speed);
  }
}

bool Baffel::isOpen() {
  return m_open;
}
