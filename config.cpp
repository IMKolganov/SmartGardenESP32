#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

struct Config {
  String ssid;
  String password;
  String device_key;
  String device_name;
};

Config config;

bool loadConfig(const char* filename, Config &config) {
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println("Не удалось открыть файл для чтения.");
    return false;
  }

  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println("Ошибка при чтении JSON файла.");
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

void readFromConfigFile(const char* filename) {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("Не удалось смонтировать файловую систему.");
    return;
  }

  if (loadConfig(filename, config)) {
    Serial.println("Настройки успешно загружены.");
    Serial.println("SSID: " + config.ssid);
  } else {
    Serial.println("Ошибка при загрузке настроек.");
  }
}