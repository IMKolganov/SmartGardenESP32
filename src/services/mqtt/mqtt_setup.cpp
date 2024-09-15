#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_setup.h"
#include "pump_control.h"

// Initialize the global instance
MQTTService mqttServiceInstance;

MQTTService::MQTTService() : mqttClient(espClient), pumpController() {
}

void MQTTService::setupMQTT(Config *config) {
    mqttClient.setServer(config->mqttServer.c_str(), config->mqttPort);
    mqttClient.setCallback(mqttCallback);

    pumpController.setupPump(config);

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
            Serial.println(" try again in 5 seconds");
            delay(5000);
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

    if (topicStr.startsWith("control/pump/")) {
        int pumpId = topicStr.substring(String("control/pump/").length()).toInt();
        String statusTopic = "status/pump/" + String(pumpId);
        PumpStatus status = mqttServiceInstance.pumpController.handleControlMessage(pumpId, message);
        Serial.print("Pump status: ");
        Serial.println(status.message);
        mqttClient.publish(statusTopic.c_str(), status.message.c_str());
    } 
    else if (topicStr.startsWith("control/sensor/")) {
        // Handle sensor control
        // mqttServiceInstance.handleSensorControl(topicStr, message);
    } 
    else {
        Serial.println("Unknown topic: " + topicStr);
    }
}
