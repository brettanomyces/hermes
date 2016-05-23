#include "catch.hpp"
#include "fakeit.hpp"
#include "TemperatureSensor.cpp"
#include "IDeviceManager.h"
#include <math.h>

TEST_CASE("valid temperatures") {
  
  fakeit::Mock<IDeviceManager> mockDeviceManager;
  TemperatureSensor sensor(1, 2, 10000, 5.0, 1024, &mockDeviceManager.get());

  SECTION("temperature 0C") {
    fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(636);
    REQUIRE(sensor.readTemperature() == Approx(0.0).epsilon(0.25));
  }

  SECTION("temperature 5C") {
    fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(573.0);
    REQUIRE(sensor.readTemperature() == Approx(5.0).epsilon(0.25));;
  }

  SECTION("temperature 10C") {
    fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(511.0);
    REQUIRE(sensor.readTemperature() == Approx(10.0).epsilon(0.25));;
  }

  SECTION("temperature 15C") {
    fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(451.0);
    REQUIRE(sensor.readTemperature() == Approx(15.0).epsilon(0.25));;
  }

  SECTION("temperature 20C") {
    fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(394.0);
    REQUIRE(sensor.readTemperature() == Approx(20.0).epsilon(0.25));;
  }

  SECTION("temperature 25C") {
    fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(342.0);
    REQUIRE(sensor.readTemperature() == Approx(25.0).epsilon(0.25));;
  }

  SECTION("temperature 30C") {
    fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(294.0);
    REQUIRE(sensor.readTemperature() == Approx(30.0).epsilon(0.25));;
  }
}

TEST_CASE("max value") {
  fakeit::Mock<IDeviceManager> mockDeviceManager;
  TemperatureSensor sensor(1, 2, 10000, 5.0, 1024, &mockDeviceManager.get());
  fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(1024);
  REQUIRE(sensor.readTemperature() == -273.15);  // absolute zero
}

TEST_CASE("min value") {
  fakeit::Mock<IDeviceManager> mockDeviceManager;
  TemperatureSensor sensor(1, 2, 10000, 5.0, 1024, &mockDeviceManager.get());
  fakeit::When(Method(mockDeviceManager, readAnalogValue)).Return(0);
  REQUIRE(std::isnan(sensor.readTemperature()) == true);
}
