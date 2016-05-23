#include "catch.hpp"
#include "TemperatureController.cpp"

TEST_CASE("get/set frSetTemp") {
  TemperatureController controller;

  controller.setFrSetTemp(10.0);
  REQUIRE(controller.getFrSetTemp() == 10.0);
}

TEST_CASE("get/set fzSetTemp") {
  TemperatureController controller;

  controller.setFzSetTemp(10.0);
  REQUIRE(controller.getFzSetTemp() == 10.0);
}

TEST_CASE("get/set difference") {
  TemperatureController controller;

  controller.setDifference(10.0);
  REQUIRE(controller.getDifference() == 10.0);
}

TEST_CASE("shouldActivateCompressor") {
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

TEST_CASE("shouldDeactivateCompressor") {
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

TEST_CASE("shouldOpenBaffel") {
  TemperatureController controller;

  controller.setFrSetTemp(15.0);
  controller.setDifference(0.5);
  
  SECTION("temp greater than frSetTemp plus diff") {
    REQUIRE(controller.shouldOpenBaffel(20.0) == true);
  }

  SECTION("temp equal to frSetTemp plus diff") {
    REQUIRE(controller.shouldOpenBaffel(15.5) == false);
  }

  SECTION("temp greater than frSetTemp but less than frSetTemp plus diff") {
    REQUIRE(controller.shouldOpenBaffel(15.25) == false);
  }

  SECTION("temp equal to frSetTemp") {
    REQUIRE(controller.shouldOpenBaffel(15.00) == false);
  }

  SECTION("temp less than frSetTemp") {
    REQUIRE(controller.shouldOpenBaffel(10.00) == false);
  }
}

TEST_CASE("shouldCloseBaffel") {
  TemperatureController controller;

  controller.setFrSetTemp(15.0);
  controller.setDifference(0.5);
  
  SECTION("temp less than frSetTemp minus diff") {
    REQUIRE(controller.shouldCloseBaffel(10.0) == true);
  }

  SECTION("temp equal to frSetTemp minus diff") {
    REQUIRE(controller.shouldCloseBaffel(14.5) == false);
  }

  SECTION("temp less than frSetTemp but greater than frSetTemp minus diff") {
    REQUIRE(controller.shouldCloseBaffel(14.75) == false);
  }

  SECTION("temp equal to frSetTemp") {
    REQUIRE(controller.shouldCloseBaffel(15.00) == false);
  }

  SECTION("temp greater than frSetTemp") {
    REQUIRE(controller.shouldCloseBaffel(20.00) == false);
  }
}

TEST_CASE("shouldActivateHeater") {
  TemperatureController controller;

  controller.setFrSetTemp(15.0);
  controller.setDifference(0.5);

  SECTION("temp less than frSetTemp and delay not active") {
    REQUIRE(controller.shouldActivateHeater(10.0, false) == true);
  }

  SECTION("temp less than frSetTemp and delay active") {
    REQUIRE(controller.shouldActivateHeater(10.0, true) == false);
  }

  SECTION("temp equal to frSetTemp minus 2 * diff") {
    REQUIRE(controller.shouldActivateHeater(14.0, false) == false);
  }

  SECTION("temp less than frSetTemp but greater than frSetTemp minus 2 * diff") {
    REQUIRE(controller.shouldActivateHeater(14.5, false) == false);
  }

  SECTION("temp equal to frSetTemp") {
    REQUIRE(controller.shouldActivateHeater(15.0, false) == false);
  }

  SECTION("temp greater than frSetTemp") {
    REQUIRE(controller.shouldActivateHeater(20.0, false) == false);
  }
}

TEST_CASE("shouldDeactivateHeater") {
  TemperatureController controller;

  controller.setFrSetTemp(15.0);
  controller.setDifference(0.5);

  SECTION("temp greater than frSetTemp and delay not active") {
    REQUIRE(controller.shouldDeactivateHeater(20.0, false) == true);
  }

  SECTION("temp greater than frSetTemp and delay active") {
    REQUIRE(controller.shouldDeactivateHeater(20.0, true) == false);
  }

  SECTION("temp greater than frSetTemp but less than frSetTemp plus diff") {
    REQUIRE(controller.shouldDeactivateHeater(15.25, false) == true);
  }

  SECTION("temp equal to frSetTemp") {
    REQUIRE(controller.shouldDeactivateHeater(15.0, false) == false);
  }

  SECTION("temp less than frSetTemp") {
    REQUIRE(controller.shouldDeactivateHeater(10.0, false) == false);
  }
}

TEST_CASE("shouldActivateFan") {
  TemperatureController controller;
  
  SECTION("compresser on, baffel open") {
    REQUIRE(controller.shouldActivateFan(true, true) == true);
  }

  SECTION("compresser on, baffel closed") {
    REQUIRE(controller.shouldActivateFan(true, false) == true);
  }

  SECTION("compresser off, baffel open") {
    REQUIRE(controller.shouldActivateFan(false, true) == true);
  }

  SECTION("compresser off, baffel closed") {
    REQUIRE(controller.shouldActivateFan(false, false) == false);
  }
}

TEST_CASE("shouldDeactivateFan") {
  TemperatureController controller;
  
  SECTION("compresser on, baffel open") {
    REQUIRE(controller.shouldDeactivateFan(true, true) == false);
  }

  SECTION("compresser on, baffel closed") {
    REQUIRE(controller.shouldDeactivateFan(true, false) == false);
  }

  SECTION("compresser off, baffel open") {
    REQUIRE(controller.shouldDeactivateFan(false, true) == false);
  }

  SECTION("compresser off, baffel closed") {
    REQUIRE(controller.shouldDeactivateFan(false, false) == true);
  }
}
