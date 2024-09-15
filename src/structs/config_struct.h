#ifndef CONFIG_STRUCT_H
#define CONFIG_STRUCT_H

#include <Arduino.h>
#include <ArduinoJson.h>
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
  int mqttTryAgain;

  String otaPassword;

  int dhtPin;
  int dhtType;

  int soilSensorPin;

  // Convert Config to JSON
  String toJson() const {
    // Create a JsonDocument with a suitable size
    JsonDocument doc;

    // Populate the JsonDocument
    doc["ssid"] = ssid;
    doc["password"] = password;
    doc["deviceKey"] = deviceKey;
    doc["deviceName"] = deviceName;

    doc["udpPort"] = udpPort;
    doc["udpDelayTimeMs"] = udpDelayTimeMs;

    doc["webServerPort"] = webServerPort;

    // Serialize Pump array
    JsonArray pumpsArray = doc.createNestedArray("pumps");
    for (int i = 0; i < 2; i++) {
      JsonObject pumpObj = pumpsArray.createNestedObject();
      pumpObj["pin"] = pumps[i].pin;
      pumpObj["lastStartTime"] = pumps[i].lastStartTime;
      pumpObj["isRunning"] = pumps[i].isRunning;
    }

    doc["pumpMaxPumpDuration"] = pumpMaxPumpDuration;
    doc["pumpMinInterval"] = pumpMinInterval;

    doc["mqttServer"] = mqttServer;
    doc["mqttPort"] = mqttPort;
    doc["mqttUser"] = mqttUser;
    doc["mqttPassword"] = mqttPassword;
    doc["mqttTryAgain"] = mqttTryAgain;

    doc["otaPassword"] = otaPassword;

    doc["dhtPin"] = dhtPin;
    doc["dhtType"] = dhtType;

    doc["soilSensorPin"] = soilSensorPin;

    // Serialize JsonDocument to a String
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
  }
};

#endif // CONFIG_STRUCT_H
