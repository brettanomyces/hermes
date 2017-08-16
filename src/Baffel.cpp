#include "Baffel.h"

Baffel::Baffel(Stepper* stepper, int steps, int speed) {
  m_steps = steps;
  m_speed = speed;
  m_open = false;
  m_stepper = stepper;
  m_stepper->setSpeed(speed);
}

void Baffel::open() {
  m_stepper->step(m_steps);
  m_open = true;
}

void Baffel::close() {
  m_stepper->step(-m_steps);
  m_open = false;
}

bool Baffel::isOpen() {
  return m_open;
}
