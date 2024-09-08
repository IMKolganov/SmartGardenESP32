#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
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

WiFiUDP udp;
const int udpPort = 6000;
const IPAddress broadcastIP = ~WiFi.subnetMask() | WiFi.gatewayIP();

// Hardcoded GUID
const char* GUID = "123e4567-e89b-12d3-a456-426614174000";

// Define pumps
Pump pumps[] = {
    {2, 0, false},  // Pump 1, pin 2
    {3, 0, false}   // Pump 2, pin 3
};

void initializeModule() {
    Serial.begin(115200);
    Serial.println("Booting");

    

    // Initialize Wi-Fi
    readFromConfigFile("/config.json");

    // Initialize DHT sensor
    setupSensors();

    // Initialize OTA
    setupOTA();

    // Initialize pumps
    setupPumps(pumps, 2);

    // Initialize MQTT
    setupMQTT();

    // Create a task for UDP Broadcast
    initializeUDPBroadcastModule();

    // Health check endpoint
    server.on("/healthcheck", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"status\":\"healthy\"}");
    });

    // Endpoint to return hardcoded GUID
    server.on("/guid", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"guid\":\"" + String(GUID) + "\"}");
    });

        // Endpoint to return hardcoded GUID
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"guid\":\"" + String(GUID) + "\"}");
    });

    server.begin();
    Serial.println("HTTP server started");
}

void udpTask(void *pvParameters) {
    while (true) {
        IPAddress ip = WiFi.localIP();
        String ipString = ip.toString();

        udp.beginPacket(broadcastIP, udpPort);
        udp.print(ipString);
        udp.endPacket();

        Serial.println("Sent IP address via Broadcast: " + ipString);

        vTaskDelay(50000 / portTICK_PERIOD_MS);
    }
}

void runModule() {
    ArduinoOTA.handle();
    for (int i = 0; i < 2; i++) {
        updatePump(&pumps[i]);
    }
    mqttClient.loop();
}
