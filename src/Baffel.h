#ifndef SRC_BAFFEL_H_
#define SRC_BAFFEL_H_

#include <Stepper.h>

class Baffel {
  public:
    Baffel(Stepper* stepper, int steps, int speed);

    void open(); 
    void close();
    bool isOpen();

  private:
    bool m_open;
    int m_steps;
    int m_speed;

    Stepper* m_stepper;
};
#endif  // SRC_BAFFEL_H_
