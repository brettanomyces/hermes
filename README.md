Hermes
======

Temperature controller for a double door fridge/freezer.

# Test

```
g++ -std=c++11 -I lib/fakeit/ -I lib/catch/ -I src/ test/UnitTest.cpp -o test/UnitTest.out
./test/UnitTest.out
```

# Build

```
platforio run --environment=esp32
```

# Flash

```
platformio --environment=esp32 --target upload
```

# Monitor

```
platforimio device monitor --baud 115200
```
