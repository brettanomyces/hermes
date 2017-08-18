#ifndef SRC_BAFFEL_H_
#define SRC_BAFFEL_H_

#include <Arduino.h>

class Baffel {
  public:
    Baffel(int in1, int in2, int in3, int in4, int steps, int delay);

    void open(); 
    void close();
    bool isOpen();

  private:
    int m_in1;
    int m_in2;
    int m_in3;
    int m_in4;
    int m_steps;
    int m_delay;
    bool m_open;

    void step(int steps);

};
#endif  // SRC_BAFFEL_H_
