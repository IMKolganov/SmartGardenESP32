#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>  // Include ArduinoJson library for JSON handling
#include "wifi_config.h"  // Include the Wi-Fi configuration file

// Server settings
AsyncWebServer server(80);

// Sensor settings
#define DHTPIN 4           // Pin connected to the DHT22
#define DHTTYPE DHT22      // Type of DHT sensor
#define SOIL_SENSOR_PIN 34 // Pin connected to the soil moisture sensor

DHT dht(DHTPIN, DHTTYPE);

// Hardcoded GUID
const char* GUID = "123e4567-e89b-12d3-a456-426614174000";

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Health check endpoint
  server.on("/healthcheck", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"status\":\"healthy\"}");
  });

  // Endpoint to return hardcoded GUID
  server.on("/guid", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"guid\":\"" + String(GUID) + "\"}");
  });

  // Handle HTTP request for temperature and humidity
  server.on("/temperature-humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    // Get data from the DHT22 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check for errors in reading from the DHT sensor
    if (isnan(temperature) || isnan(humidity)) {
      request->send(500, "application/json", "{\"error\":\"Failed to read from DHT sensor\"}");
      return;
    }

    // Prepare JSON response
    String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
    request->send(200, "application/json", json);
  });

  // Handle HTTP request for soil moisture
  server.on("/soil-moisture", HTTP_GET, [](AsyncWebServerRequest *request){
    // Get data from the soil moisture sensor
    int soilMoisture = analogRead(SOIL_SENSOR_PIN);

    // Prepare JSON response
    String json = "{\"soilMoisture\":" + String(soilMoisture) + "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  // Check for input from the serial port (USB)
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (input == "GET /temperature-humidity") {
      // Get data from the DHT22 sensor
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      // Check for errors in reading from the DHT sensor
      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("{\"error\":\"Failed to read from DHT sensor\"}");
        return;
      }

      // Prepare JSON response
      String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
      Serial.println(json);
    }

    if (input == "GET /soil-moisture") {
      // Get data from the soil moisture sensor
      int soilMoisture = analogRead(SOIL_SENSOR_PIN);

      // Prepare JSON response
      String json = "{\"soilMoisture\":" + String(soilMoisture) + "}";
      Serial.println(json);
    }

    if (input == "GET /guid") {
      // Return hardcoded GUID
      String json = "{\"guid\":\"" + String(GUID) + "\"}";
      Serial.println(json);
    }

    if (input == "GET /healthcheck") {
      // Return health status
      Serial.println("{\"status\":\"healthy\"}");
    }
  }
}
