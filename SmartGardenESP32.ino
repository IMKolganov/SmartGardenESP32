#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
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
  Serial.println("Booting");

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize DHT sensor
  dht.begin();

  // Initialize OTA
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
  Serial.println("OTA Ready");

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
  Serial.println("HTTP server started");
}

void loop() {
  // Handle OTA updates
  ArduinoOTA.handle();

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
