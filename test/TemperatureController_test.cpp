#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "TemperatureController.cpp"

TEST_CASE("test shouldActivateCompressor") {
  TemperatureController controller;

  controller.setFzSetTemp(5.0);
  controller.setDifference(0.5);

  SECTION("temp greater than fzSetTemp and delay not active") {
    REQUIRE(controller.shouldActivateCompressor(7.0, false) == true);
  }

  SECTION("temp greater than fzSetTemp by less than diff") {
   REQUIRE(controller.shouldActivateCompressor(5.25, false) == false);
  }

  SECTION("temp equal to fzSetTemp plus diff") {
   REQUIRE(controller.shouldActivateCompressor(5.5, false) == false);
  }

  SECTION("temp greater than fzSetTemp and delay active") {
    REQUIRE(controller.shouldActivateCompressor(7.0, true) == false);
  }

  SECTION("temp less than fzSetTemp and delay active") {
    REQUIRE(controller.shouldActivateCompressor(3.0, true) == false);
  }

  SECTION("temp less than fzSetTemp and delay not active") {
    REQUIRE(controller.shouldActivateCompressor(3.0, false) == false);
  }

  SECTION("temp greater than fzSetTemp by less than diff") {
   REQUIRE(controller.shouldActivateCompressor(5.25, false) == false);
  }

}

TEST_CASE("test shouldDeactivateCompressor") {
  TemperatureController controller;

  controller.setFzSetTemp(5.0);
  controller.setDifference(0.5);

  SECTION("temp less than fzSetTemp and delay not active") {
    REQUIRE(controller.shouldDeactivateCompressor(3.0, false) == true);
  }

  SECTION("temp less than fzSetTemp by less than diff") {
    REQUIRE(controller.shouldDeactivateCompressor(4.75, false) == false);
  }

  SECTION("temp equal to fzSetTemp minus diff") {
    REQUIRE(controller.shouldDeactivateCompressor(4.5, false) == false);
  }

  SECTION("temp less than fzSetTemp and delay active") {
    REQUIRE(controller.shouldDeactivateCompressor(3.0, true) == false);
  }

  SECTION("temp greater than fzSetTemp and delay not active") {
    REQUIRE(controller.shouldDeactivateCompressor(7.0, false) == false);
  }

  SECTION("temp greater than fzSetTemp and delay active") {
    REQUIRE(controller.shouldDeactivateCompressor(7.0, true) == false);
  }
}
