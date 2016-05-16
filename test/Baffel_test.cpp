#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "fakeit.hpp"

#include "Baffel.cpp"
#include "IDeviceManager.h"

TEST_CASE("isOpen()") {
  fakeit::Mock<IDeviceManager> mock;
  fakeit::Fake(Method(mock, openBaffel));
  fakeit::Fake(Method(mock, closeBaffel));
  Baffel baffel(1, 2, 3, 4, 100, 100, &mock.get());

  SECTION("initial state") {
    REQUIRE(baffel.isOpen() == false);
  }

  SECTION("after open() called") {
    baffel.open();
    REQUIRE(baffel.isOpen() == true);
  }

  SECTION("after close() called") {
    baffel.close();
    REQUIRE(baffel.isOpen() == false);
  }
}
