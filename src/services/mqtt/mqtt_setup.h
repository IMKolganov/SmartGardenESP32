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

    PubSubClient mqttClient;
    PumpController pumpController;

    void setupMQTT(Config *config);

    void sendLog(const String& message);

private:
    WiFiClient espClient;
    DhtController dhtController;
    SoilMoistureController soilMoistureController;

    // Static callback function
    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    // Method to call from static callback
    void processMessage(char* topic, byte* payload, unsigned int length);

    void sendMessage(const String& topic, const String& message);
};

extern MQTTService mqttServiceInstance;  // Declare the global instance

#endif // MQTT_SETUP_H
