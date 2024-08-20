#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include "wifi_config.h"  // Include the Wi-Fi configuration file

// Server settings
AsyncWebServer server(80);

// Sensor settings
#define DHTPIN 4           // Pin connected to the DHT22
#define DHTTYPE DHT22      // Type of DHT sensor
#define SOIL_SENSOR_PIN 34 // Pin connected to the soil moisture sensor

DHT dht(DHTPIN, DHTTYPE);

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

  // Handle HTTP requests
  server.on("/environment", HTTP_GET, [](AsyncWebServerRequest *request){
    // Get data from the DHT22 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check for errors in reading from the DHT sensor
    if (isnan(temperature) || isnan(humidity)) {
      request->send(500, "application/json", "{\"error\":\"Failed to read from DHT sensor\"}");
      return;
    }

    // Get data from the soil moisture sensor
    int soilMoisture = analogRead(SOIL_SENSOR_PIN);

    // Prepare JSON response
    String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"soilMoisture\":" + String(soilMoisture) + "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  // Check for input from the serial port (USB)
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input == "GET /environment") {
      // Get data from the DHT22 sensor
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      // Check for errors in reading from the DHT sensor
      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("{\"error\":\"Failed to read from DHT sensor\"}");
        return;
      }

      // Get data from the soil moisture sensor
      int soilMoisture = analogRead(SOIL_SENSOR_PIN);

      // Prepare JSON response
      String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"soilMoisture\":" + String(soilMoisture) + "}";
      Serial.println(json);
    }
  }
}
