#ifndef CONFIG_STRUCT_H
#define CONFIG_STRUCT_H

#include <Arduino.h>
#include "pump_struct.h"

struct Config {
  String ssid;
  String password;
  String deviceKey;
  String deviceName;

  int udpPort;
  int udpDelayTimeMs;

  int webServerPort;

  Pump pumps[2];
  int pumpMaxPumpDuration;
  int pumpMinInterval;

  String mqttServer;
  int mqttPort;
  String mqttUser;
  String mqttPassword;

  String otaPassword;
};

#endif // CONFIG_STRUCT_H
