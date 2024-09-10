#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

#include "config.h"
#include "wifi_setup.h"
#include "ota_setup.h"
#include "sensors.h"
#include "mqtt_setup.h"
#include "udp_module.h"
#include "web_server_request_setup.h"

const char* filename = "/config.json";

void initializeModule() {
    Serial.begin(115200);
    Serial.println("Booting");

    Config config = readFromConfigFile(filename);

    // Initialize Wi-Fi
    setupWiFi(config.ssid.c_str(), config.password.c_str());

    // Initialize web server
    setupWebServerRequestsSetup(config.webServerPort, config.deviceName.c_str(), config.deviceKey.c_str());

    setupSensors(&config);

    // Initialize OTA
    setupOTA(config.otaPassword.c_str());

    // Initialize MQTT
    setupMQTT(&config);

    // Create a task for UDP Broadcast
    initializeUDPBroadcastModule(config.udpPort, config.udpDelayTimeMs);

    Serial.println("HTTP server started");
}

void runModule() {
    ArduinoOTA.handle();
    mqttClient.loop();
}
