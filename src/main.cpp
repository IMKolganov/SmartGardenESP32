#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

#include "config.h"
#include "wifi_setup.h"
#include "ota_setup.h"
#include "sensors.h"
#include "pump_control.h"
#include "mqtt_setup.h"
#include "udp_module.h"

// Server settings
AsyncWebServer server(80);

const char* filename = "/config.json";

// Hardcoded GUID

// Define pumps
Pump pumps[] = {
    {2, 0, false},  // Pump 1, pin 2
    {3, 0, false}   // Pump 2, pin 3
};

void initializeModule() {
    Serial.begin(115200);
    Serial.println("Booting");

    Config config = readFromConfigFile(filename);

    // Initialize Wi-Fi
    setupWiFi(config.ssid.c_str(), config.password.c_str());

    // Initialize DHT sensor
    setupSensors();

    // Initialize OTA
    setupOTA();

    // Initialize pumps
    setupPumps(pumps, 2);

    // Initialize MQTT
    setupMQTT();

    // Create a task for UDP Broadcast
    initializeUDPBroadcastModule(config.udpPort, config.udpDelayTimeMs);

    // Health check endpoint
    server.on("/healthcheck", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"status\":\"healthy\"}");
    });

    // Endpoint to return hardcoded GUID
    server.on("/guid", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"guid\":\"" + String("GUID") + "\"}");
    });

        // Endpoint to return hardcoded GUID
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"guid\":\"" + String("GUID") + "\"}");
    });

    server.begin();
    Serial.println("HTTP server started");
}

void runModule() {
    ArduinoOTA.handle();
    for (int i = 0; i < 2; i++) {
        updatePump(&pumps[i]);
    }
    mqttClient.loop();
}
