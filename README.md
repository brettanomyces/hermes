Hermes
======

Temperature controller for a double door fridge/freezer.

# Test


```
g++ -std=c++11 -I lib/fakeit/ -I lib/catch/ -I src/ test/UnitTest.cpp -o test/UnitTest.out
./test/UnitTest.out
```

# Build

To build for Arduino:


```
platforio run
```


To build for Particle Photon, uncomment `#define PARTICLE` in `src/hermes.ino` then:


```
particle compile photon src/*
```

Particle Photon has a different V_OUT (used to read temperature), ADC steps and clock speed. 

# Flash

To flash Arduino:


```
platformio --target upload
```


To flash Photon:


```
particle flash <device> src/*
```

# View State


```
./srcipts/status.sh
```
