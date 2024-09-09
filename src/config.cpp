#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <config.h>

Config config;

bool loadConfig(const char* filename, Config &config) {
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println("Не удалось открыть файл для чтения.");
    return false;
  }

  JsonDocument doc;

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

Config readFromConfigFile(const char* filename) {
  Config config;

  // Монтируем файловую систему SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Не удалось смонтировать файловую систему.");
    return config; // Вернём пустую конфигурацию
  }

  // Загружаем настройки из файла
  if (loadConfig(filename, config)) {
    Serial.println("Настройки успешно загружены.");
    Serial.println("SSID: " + config.ssid);
  } else {
    Serial.println("Ошибка при загрузке настроек.");
  }

  return config; // Возвращаем загруженную конфигурацию
}