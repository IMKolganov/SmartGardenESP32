#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_setup.h"
#include "pump_controller.h"
#include "dht_controller.h"

// Initialize the global instance
MQTTService mqttServiceInstance;

MQTTService::MQTTService() : mqttClient(espClient), pumpController(), dhtController() {
}

void MQTTService::setupMQTT(Config *config) {
    mqttClient.setServer(config->mqttServer.c_str(), config->mqttPort);
    mqttClient.setCallback(mqttCallback);

    pumpController.setupPump(config);//todo: move from here
    dhtController.setupDht(config);

    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT... IP: ");
        Serial.println(config->mqttServer);
        Serial.print(" Port: ");
        Serial.print(config->mqttPort);
        if (mqttClient.connect("ESP32Client", config->mqttUser.c_str(), config->mqttPassword.c_str())) {
            Serial.println(" connected");
            mqttClient.subscribe("control/pump/#");
            mqttClient.subscribe("control/sensor/#");
        } else {
            Serial.print(" failed, rc=");
            Serial.print(mqttClient.state());
            Serial.print(" try again in ");
            Serial.print(config->mqttTryAgain);
            Serial.println(" miliseconds");
            delay(config->mqttTryAgain);
        }
    }
}

// Static callback function
void MQTTService::mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (&mqttServiceInstance) {
        mqttServiceInstance.processMessage(topic, payload, length);
    }
}

void MQTTService::processMessage(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message payload: ");
    Serial.println(message);

    String topicStr = String(topic);
    String statusTopic = "";

    if (topicStr.startsWith("control/pump/")) {
        int pumpId = topicStr.substring(String("control/pump/").length()).toInt();
        String statusTopic = "status/pump/" + String(pumpId);
        PumpStatus status = mqttServiceInstance.pumpController.handleControlMessage(pumpId, message);
        Serial.print("Pump status: ");
        Serial.println(status.message);
        mqttClient.publish(statusTopic.c_str(), status.message.c_str());
    } 
    else if (topicStr.startsWith("control/dht/")) {
        DhtStatus status = mqttServiceInstance.dhtController.handleControlMessage(message);
        statusTopic = "status/dht/";
        Serial.print("Dht status: ");
        String payload = status.toJson();
        mqttClient.publish(statusTopic.c_str(), payload.c_str());
    } 
    else {
        Serial.println("Unknown topic: " + topicStr);
    }
}
