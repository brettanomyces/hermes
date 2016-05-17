#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "fakeit.hpp"

#include "Baffel.cpp"
#include "IDeviceManager.h"

TEST_CASE("isOpen()") {
  fakeit::Mock<IDeviceManager> mock;
  fakeit::Fake(Method(mock, openBaffel));
  fakeit::Fake(Method(mock, closeBaffel));
  Baffel baffel(1, 2, 3, 4, 10, 100, &mock.get());

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

TEST_CASE("verifty correct parameters pass to device manager") {
  fakeit::Mock<IDeviceManager> mock;
  fakeit::Fake(Method(mock, openBaffel));
  fakeit::Fake(Method(mock, closeBaffel));
  Baffel baffel(1, 2, 3, 4, 10, 100, &mock.get());

  SECTION("openBaffel()") {
    baffel.open();
    fakeit::Verify(Method(mock, openBaffel).Using(1, 2, 3, 4, 10, 100));
  }

  SECTION("closeBaffel()") {
    baffel.close();
    fakeit::Verify(Method(mock, closeBaffel).Using(1, 2, 3, 4, 10, 100));
  }
}
