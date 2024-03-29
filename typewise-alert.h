#pragma once
#include <map>
#include <utility>
#include<string>
#include<stdlib.h>

typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

typedef std::pair< double,double > temperatureCheck;
typedef std::map< CoolingType,temperatureCheck > temperatureCheckMap;
typedef std::map<BreachType,std::string> temperatureStatus;

temperatureCheckMap getTemperatureRangeList();
temperatureStatus getTemperatureStatusMessage();

void printOnConsole(std::string message);
std::string sendToController(BreachType breachType);
std::string sendToEmail(BreachType breachType,temperatureStatus temperatureStatusMessageList);
BreachType inferBreach(double value, double lowerLimit, double upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC,temperatureCheckMap temperatureRangeList);
void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

