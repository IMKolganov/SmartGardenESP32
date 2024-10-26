#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include <WiFiClient.h>
#include "configuries/config.h"
#include "services/devices/pump_controller.h"
#include "services/sensors/dht_controller.h"
#include "services/sensors/soil_moisture_controller.h"

class MQTTService {
public:
    MQTTService();
    PumpController pumpController;

    void setupMQTT(Config *config);
    void loop();  // Add loop method

    void sendLog(const String& message);

private:
    PubSubClient mqttClient;
    WiFiClient espClient;
    
    DhtController dhtController;
    SoilMoistureController soilMoistureController;

    Config *config;  // Pointer to store the Config object
    unsigned long lastReconnectAttempt = 0;

    // Static callback function
    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    // Method to call from static callback
    void processMessage(char* topic, byte* payload, unsigned int length);

    void sendMessage(const String& topic, const String& message);
    bool connectToMQTT();  // Add connectToMQTT method
};

extern MQTTService mqttServiceInstance;  // Declare the global instance

#endif // MQTT_SETUP_H