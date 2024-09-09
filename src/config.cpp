#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <config.h>

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
  config.device_key = doc["device_key"].as<String>();
  config.device_name = doc["device_name"].as<String>();

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
