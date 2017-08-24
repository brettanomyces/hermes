#include "Baffel.h"

Baffel::Baffel(int in1, int in2, int in3, int in4, int steps, int delay) {
  m_in1 = in1;
  m_in2 = in2;
  m_in3 = in3;
  m_in4 = in4;
  m_steps = steps;
  m_delay = delay;
  m_open = false;
}

void Baffel::open() {
  this->step(-m_steps);
  m_open = true;
}

void Baffel::close() {
  this->step(m_steps);
  m_open = false;
}

bool Baffel::isOpen() {
  return m_open;
}

void Baffel::step(int steps) {

  int remaining = abs(steps);
  unsigned long now = micros();
  unsigned long then = now;

  while(remaining > 0) {
    now = micros();
    if (now - then >= 10000) {
      then = now;
      int step = abs(steps) % 4;
      switch (step) {
        case 0:  // 1010
          digitalWrite(m_in1, 1);
          digitalWrite(m_in2, 0);
          digitalWrite(m_in3, 1);
          digitalWrite(m_in4, 0);
        break;
        case 1:  // 0110
          digitalWrite(m_in1, 0);
          digitalWrite(m_in2, 1);
          digitalWrite(m_in3, 1);
          digitalWrite(m_in4, 0);
        break;
        case 2:  // 0101
          digitalWrite(m_in1, 0);
          digitalWrite(m_in2, 1);
          digitalWrite(m_in3, 0);
          digitalWrite(m_in4, 1);
        break;
        case 3:  // 1001
          digitalWrite(m_in1, 1);
          digitalWrite(m_in2, 0);
          digitalWrite(m_in3, 0);
          digitalWrite(m_in4, 1);
        break;
      }
      steps++;
      remaining--;
    }
  }
}
