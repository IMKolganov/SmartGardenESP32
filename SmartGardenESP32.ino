#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "wifi_config.h"  // Include the Wi-Fi configuration file

#include "wifi_setup.h"
#include "ota_setup.h"
#include "sensors.h"

// Server settings
AsyncWebServer server(80);

WiFiUDP udp;
const int udpPort = 4000;
const IPAddress broadcastIP = ~WiFi.subnetMask() | WiFi.gatewayIP();

// Hardcoded GUID
const char* GUID = "123e4567-e89b-12d3-a456-426614174000";

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  // Initialize Wi-Fi
  setupWiFi();

  // Initialize DHT sensor
  setupSensors();

  // Initialize OTA
  setupOTA();

  // Create a task for UDP Broadcast
  xTaskCreatePinnedToCore(
    udpTask,    // Pointer to the task function
    "UDP Task", // Task name
    10000,      // Task stack size
    NULL,       // Parameters for the task function
    1,          // Task priority
    NULL,       // Task handle
    1           // Processor core (0 or 1)
  );

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
    handleTemperatureHumidityRequest(request);
  });

  // Handle HTTP request for soil moisture
  server.on("/soil-moisture", HTTP_GET, [](AsyncWebServerRequest *request){
    handleSoilMoistureRequest(request);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void udpTask(void *pvParameters) {
  while (true) {
    // Send IP UDP Broadcast
    IPAddress ip = WiFi.localIP();
    String ipString = ip.toString();

    udp.beginPacket(broadcastIP, udpPort);
    udp.print(ipString);
    udp.endPacket();

    Serial.println("Sent IP address via Broadcast: " + ipString);

    // waiting
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void loop() {
  // Handle OTA updates
  ArduinoOTA.handle();

  // Check for input from the serial port (USB)
  handleSerialInput();
}
