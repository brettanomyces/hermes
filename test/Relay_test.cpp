#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "fakeit.hpp"

#include "Delay.cpp"
#include "Relay.cpp"
#include "IDeviceManager.h"

TEST_CASE("isActive") {
  fakeit::Mock<IDeviceManager> mock;
  fakeit::When(Method(mock, getMillis)).AlwaysReturn(0);
  fakeit::Fake(Method(mock, activateRelay));
  fakeit::Fake(Method(mock, deactivateRelay));
  Relay relay(1, 0, true, &mock.get());

  SECTION("initial state") {
    REQUIRE(relay.isActive() == false);
  }

  SECTION("after activate") {
    relay.activate();
    REQUIRE(relay.isActive() == true);
  }

  SECTION("after deactivate") {
    relay.deactivate();
    REQUIRE(relay.isActive() == false);
  }
}

TEST_CASE("isWaiting") {
  fakeit::Mock<IDeviceManager> mock;
  fakeit::When(Method(mock, getMillis)).Return(0);  // called in Relay constructor
  fakeit::Fake(Method(mock, activateRelay));
  fakeit::Fake(Method(mock, deactivateRelay));
  Relay relay(1, 500, true, &mock.get());

  SECTION("millis equals zero") {
    REQUIRE(relay.isWaiting() == true);
  }

  SECTION("millis less than delay") {
    fakeit::When(Method(mock, getMillis)).Return(250);
    REQUIRE(relay.isWaiting() == true);
  }

  SECTION("millis equal to delay") {
    fakeit::When(Method(mock, getMillis)).Return(500);
    REQUIRE(relay.isWaiting() == false);
  }

  SECTION("millis greater than delay") {
    fakeit::When(Method(mock, getMillis)).Return(750);
    REQUIRE(relay.isWaiting() == false);
  }
}
