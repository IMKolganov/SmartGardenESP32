#include <Arduino.h>
#include "sensors.h"

#define DHTPIN 4           // Pin connected to the DHT22
#define DHTTYPE DHT11      // Type of DHT sensor
#define SOIL_SENSOR_PIN 34 // Pin connected to the soil moisture sensor

extern const char* GUID;   // Externally defined GUID

DHT dht(DHTPIN, DHTTYPE);

void setupSensors() {
  dht.begin();
}

void handleTemperatureHumidityRequest(AsyncWebServerRequest *request) {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    request->send(500, "application/json", "{\"error\":\"Failed to read from DHT sensor\"}");
    return;
  }

  String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
  request->send(200, "application/json", json);
}

void handleSoilMoistureRequest(AsyncWebServerRequest *request) {
  int soilMoisture = analogRead(SOIL_SENSOR_PIN);

  String json = "{\"soilMoisture\":" + String(soilMoisture) + "}";
  request->send(200, "application/json", json);
}

void handleSerialInput() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (input == "GET /temperature-humidity") {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("{\"error\":\"Failed to read from DHT sensor\"}");
        return;
      }

      String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
      Serial.println(json);
    }

    if (input == "GET /soil-moisture") {
      int soilMoisture = analogRead(SOIL_SENSOR_PIN);

      String json = "{\"soilMoisture\":" + String(soilMoisture) + "}";
      Serial.println(json);
    }

    if (input == "GET /guid") {
      String json = "{\"guid\":\"" + String(GUID) + "\"}";
      Serial.println(json);
    }

    if (input == "GET /healthcheck") {
      Serial.println("{\"status\":\"healthy\"}");
    }
  }
}
