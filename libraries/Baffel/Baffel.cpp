#include <Baffel.h>

Baffel::Baffel(DeviceManager deviceManager){
  m_dm = deviceManager;
  m_open = false;
}

// attempt to open baffel. does nothing if baffel is already open
void Baffel::open() {
  if (!isOpen()) {
    m_dm.openBaffel();
  }
}

// attempt to close baffel. does nothing if baffel is already closed
void Baffel::close() {
  if (isOpen()) {
    m_dm.closeBaffel();
  }
}

bool Baffel::isOpen() {
  return m_open;
}
