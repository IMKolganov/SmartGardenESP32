#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include "config.h"
#include "pump_control.h"

// MQTT client
extern WiFiClient espClient;
extern PubSubClient mqttClient;

void setupMQTT(Config *config);
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif
