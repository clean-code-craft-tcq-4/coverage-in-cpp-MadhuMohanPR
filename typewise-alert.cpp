#include "typewise-alert.h"
#include <stdio.h>
#include <string>
#include <sstream>

void printOnConsole(std::string message)
{
    printf("%s\n",message.c_str());
}

temperatureCheckMap getTemperatureRangeList()
{
    temperatureCheckMap tempRangeMap;
    tempRangeMap.insert(std::make_pair(PASSIVE_COOLING,std::make_pair(0,35)));
    tempRangeMap.insert(std::make_pair(HI_ACTIVE_COOLING,std::make_pair(0,45)));
    tempRangeMap.insert(std::make_pair(MED_ACTIVE_COOLING,std::make_pair(0,40)));
    return tempRangeMap;
}

temperatureStatus getTemperatureStatusMessage()
{
     temperatureStatus tempStatusList;
     tempStatusList.insert(std::make_pair(TOO_LOW,"Temperature is too low")),
     tempStatusList.insert(std::make_pair(TOO_HIGH,"Temperature is too high")),
     tempStatusList.insert(std::make_pair(NORMAL,"Temperature is normal"));
     return tempStatusList;
};

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  return (value < lowerLimit ? TOO_LOW : (value > upperLimit ? TOO_HIGH : NORMAL));
}

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC,temperatureCheckMap temperatureRangeList) {
  return inferBreach(temperatureInC, temperatureRangeList[coolingType].first, temperatureRangeList[coolingType].second);
}

std::string sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  std::stringstream outputMessage;
  outputMessage << std::hex << header << " : " << std::hex << breachType;
  return outputMessage.str();
}

std::string sendToEmail(BreachType breachType,temperatureStatus temperatureStatusMessageList) {
  std::string outputMessage = "";
  std::string recepient = "a.b@c.com";
  if (breachType != NORMAL)
  {
  std::string tempStatusMessage = (temperatureStatusMessageList.find(breachType))->second;
  outputMessage = "To: " + recepient + "\n" + "Hi," + tempStatusMessage;
  }
  return outputMessage;
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
		
    temperatureCheckMap tempRangeMap=getTemperatureRangeList();
    temperatureStatus tempStatusList=getTemperatureStatusMessage();
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC,tempRangeMap);

    std::string message=(alertTarget ==TO_CONTROLLER)?sendToController(breachType):sendToEmail(breachType,tempStatusList);
    printOnConsole(message);
}

