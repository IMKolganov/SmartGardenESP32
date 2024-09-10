#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <config.h>
#include <pump_struct.h>

Config config;

bool loadConfig(const char* filename, Config &config) {
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open file for reading.");
    return false;
  }else{
    
    Serial.print("File is opened. Size: ");
    Serial.print(file.size());
    Serial.print(" FileName: ");
    Serial.println(file.name());
  }

DynamicJsonDocument doc(1024); // Adjust the size as needed

DeserializationError error = deserializeJson(doc, file);
if (error) {
    Serial.print("Error reading JSON file: ");
    Serial.println(error.c_str()); // Print the specific error message
    file.close();
    return false;
}

  config.ssid = doc["ssid"].as<String>();
  config.password = doc["password"].as<String>();
  config.deviceKey = doc["deviceKey"].as<String>();
  config.deviceName = doc["deviceName"].as<String>();

  // Load pumps from the JSON array
  JsonArray pumpsArray = doc["pumps"].as<JsonArray>();
  int i = 0;
  for (JsonObject pumpObj : pumpsArray) {
    if (i >= 2) break;  // Limit to 2 pumps; adjust if you have more
    config.pumps[i].pin = pumpObj["pin"].as<int>();
    config.pumps[i].lastStartTime = pumpObj["lastStartTime"].as<unsigned long>();
    config.pumps[i].isRunning = pumpObj["isRunning"].as<bool>();
    i++;
  }

  file.close();
  return true;
}

Config readFromConfigFile(const char* filename) {
  Config config;

  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount file system.");
    return config;
  }

  // Load settings from file
  if (loadConfig(filename, config)) {
    Serial.println("Settings successfully loaded.");
    Serial.println("SSID: " + config.ssid);
  } else {
    Serial.println("Error loading settings.");
  }

  return config;
}
