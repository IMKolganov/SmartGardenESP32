#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

#include "configuries/config.h"
#include "services/wifi/wifi_setup.h"
#include "services/ota/ota_setup.h"
#include "services/mqtt/mqtt_setup.h"
#include "services/broadcast/udp_module.h"
#include "services/webserver/web_server_request_setup.h"

const char* filename = "/config.json";

void initializeModule() {
    Serial.begin(115200);
    Serial.println("Booting");

    Config config = readFromConfigFile(filename);

    // Initialize Wi-Fi
    setupWiFi(config.ssid.c_str(), config.password.c_str());

    // Initialize web server
    setupWebServerRequestsSetup(config.webServerPort, config.deviceName.c_str(), config.deviceKey.c_str());

    // Initialize OTA
    setupOTA(config.otaPassword.c_str());

    // Initialize MQTT
    mqttServiceInstance.setupMQTT(&config);

    // Create a task for UDP Broadcast
    initializeUDPBroadcastModule(config.udpPort, config.udpDelayTimeMs);

    Serial.println("HTTP server started");
    mqttServiceInstance.sendLog("HTTP server started");
    // mqttServiceInstance.sendLog(config.toJson());
}

void runModule() {
    ArduinoOTA.handle();
    mqttServiceInstance.mqttClient.loop();
    mqttServiceInstance.pumpController.updateAllPumps();
}
