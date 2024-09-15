#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include <WiFiClient.h>
#include "configuries/config.h"
#include "pump_controller.h"
#include "dht_controller.h"

class MQTTService {
public:
    MQTTService();

    PubSubClient mqttClient;
    PumpController pumpController;
    DhtController dhtController;

    void setupMQTT(Config *config);

    void sendLog(const String& message);

private:
    WiFiClient espClient;    

    // Static callback function
    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    // Method to call from static callback
    void processMessage(char* topic, byte* payload, unsigned int length);

    void sendMessage(const String& topic, const String& message);
};

extern MQTTService mqttServiceInstance;  // Declare the global instance

#endif // MQTT_SETUP_H
