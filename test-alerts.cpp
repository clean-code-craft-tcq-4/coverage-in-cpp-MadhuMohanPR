#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
  REQUIRE(inferBreach(35, 20, 30) == TOO_HIGH);
  REQUIRE(inferBreach(25, 20, 30) == NORMAL);
}

TEST_CASE("get the temperature limits for each cooling type") {
  temperatureCheckMap tempRangeList=getTemperatureRangeList();
  REQUIRE(tempRangeList.size() == 3);
  REQUIRE(tempRangeList.begin()->first == PASSIVE_COOLING);
  REQUIRE(tempRangeList[PASSIVE_COOLING].first == 0);
  REQUIRE(tempRangeList[PASSIVE_COOLING].second == 35);
}


TEST_CASE("get temperature status message list") {
  temperatureStatus tempStatusList=getTemperatureStatusMessage();
  REQUIRE(tempStatusList.size() == 3);
  REQUIRE(tempStatusList.begin()->first == NORMAL);
  REQUIRE(tempStatusList.begin()->second == "Temperature is normal");
}

TEST_CASE("classify the given temperature based on given ranges") {
  temperatureCheckMap tempRangeList=getTemperatureRangeList();
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING,-10,tempRangeList) == TOO_LOW);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING,70,tempRangeList) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING,20,tempRangeList) == NORMAL);
}

TEST_CASE("Retrieve the message sent from email") {
  temperatureStatus tempStatusList=getTemperatureStatusMessage();
	std::string recepient = "a.b@c.com";
  std::string status="Temperature is too high";
	std::string expectedMessage="To: " + recepient + "\n" + "Hi," + status;
  REQUIRE(sendToEmail(TOO_HIGH,tempStatusList) ==expectedMessage);
}

TEST_CASE("Retrieve the message sent from controller") {
  temperatureStatus tempStatusList=getTemperatureStatusMessage();
	std::string expectedMessage="feed : 2";
  REQUIRE(sendToController(TOO_HIGH) ==expectedMessage);
	printOnConsole(sendToController(TOO_HIGH));
}

TEST_CASE("Test temperature checker and alert functionality") {
  BatteryCharacter batteryType;
  batteryType.coolingType = PASSIVE_COOLING;
  checkAndAlert(TO_EMAIL, batteryType, 50);
}

