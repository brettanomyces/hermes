#include "catch.hpp"
#include "fakeit.hpp"
#include "Delay.cpp"
#include "IDeviceManager.h"

TEST_CASE("no delay") {

  fakeit::Mock<IDeviceManager> mockDeviceManager;
  Delay delay(0, &mockDeviceManager.get());

  SECTION("after 0 millis") {
    fakeit::When(Method(mockDeviceManager, getMillis)).Return(0);
    REQUIRE(delay.ok() == true);
  }

  SECTION("after 500 millis") {
    fakeit::When(Method(mockDeviceManager, getMillis)).Return(500);
    REQUIRE(delay.ok() == true);
  }

  SECTION("after 4294967295 millis") {
    fakeit::When(Method(mockDeviceManager, getMillis)).Return(4294967295);
    REQUIRE(delay.ok() == true);
  }
}


TEST_CASE("1000 delay") {

  fakeit::Mock<IDeviceManager> mockDeviceManager;
  Delay delay(1000, &mockDeviceManager.get());

  SECTION("after 0 millis") {
    fakeit::When(Method(mockDeviceManager, getMillis)).Return(0);
    REQUIRE(delay.ok() == false);
  }

  SECTION("after 500 millis") {
    fakeit::When(Method(mockDeviceManager, getMillis)).Return(500);
    REQUIRE(delay.ok() == false);
  }

  SECTION("after 1000 millis") {
    fakeit::When(Method(mockDeviceManager, getMillis)).Return(1000);
    REQUIRE(delay.ok() == true);
  }

  SECTION("after 4294967295 millis") {
    fakeit::When(Method(mockDeviceManager, getMillis)).Return(4294967295);
    REQUIRE(delay.ok() == true);
  }
}
