#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include "pump_control.h"

// MQTT settings
extern const char* mqttServer;
extern const int mqttPort;
extern const char* mqttUser;
extern const char* mqttPassword;

// MQTT client
extern WiFiClient espClient;
extern PubSubClient mqttClient;

// Прототипы функций
void setupMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif
