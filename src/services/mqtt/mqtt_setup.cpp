#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_setup.h"
#include "services/devices/pump_controller.h"
#include "services/sensors/dht_controller.h"
#include "services/sensors/soil_moisture_controller.h"

// Initialize the global instance
MQTTService mqttServiceInstance;

MQTTService::MQTTService() : mqttClient(espClient), pumpController(), dhtController() {
}

void MQTTService::setupMQTT(Config *config) {
    mqttClient.setServer(config->mqttServer.c_str(), config->mqttPort);
    mqttClient.setCallback(mqttCallback);

    pumpController.setupPump(config);//todo: move from here
    dhtController.setupDht(config);
    soilMoistureController.setupSoilMoisture(config);

    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT... IP: ");
        Serial.println(config->mqttServer);
        Serial.print(" Port: ");
        Serial.print(config->mqttPort);
        if (mqttClient.connect("ESP32Client", config->mqttUser.c_str(), config->mqttPassword.c_str())) {
            Serial.println(" connected");
            mqttClient.subscribe("control/pump/#");
            mqttClient.subscribe("control/dht/");
            mqttClient.subscribe("control/soil-moisture/");
            
            mqttClient.subscribe("status/esp32/smartgarden/");
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

void MQTTService::sendMessage(const String& topic, const String& message) {
    // Publish the message to the specified topic
    mqttClient.publish(topic.c_str(), message.c_str());
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

    String jsonResponse = "";

    if (topicStr.startsWith("control/pump/")) {
        int pumpId = topicStr.substring(String("control/pump/").length()).toInt();
        String statusTopic = "status/pump/" + String(pumpId);
        PumpStatus pumpStatus = mqttServiceInstance.pumpController.handleControlMessage(pumpId, message);
        Serial.print("Pump status: ");
        jsonResponse = pumpStatus.toJson();
        Serial.println(jsonResponse.c_str());
        sendMessage(statusTopic.c_str(), jsonResponse.c_str());
    } 
    else if (topicStr.startsWith("control/dht/")) {
        DhtStatus dhtStatus = mqttServiceInstance.dhtController.handleControlMessage(message);
        statusTopic = "status/dht/";
        Serial.print("Dht status: ");
        jsonResponse = dhtStatus.toJson();
        Serial.println(jsonResponse.c_str());
        sendMessage(statusTopic.c_str(), jsonResponse.c_str());
    }
    else if (topicStr.startsWith("control/soil-moisture/")) {
        SoilMoistureStatus soilMoistureStatus = mqttServiceInstance.soilMoistureController.handleControlMessage(message);
        statusTopic = "status/soil-moisture/";
        Serial.print("Soil moisture status: ");
        jsonResponse = soilMoistureStatus.toJson();
        Serial.println(jsonResponse.c_str());
        sendMessage(statusTopic.c_str(), jsonResponse.c_str());
    }
    else {
        Serial.println("Unknown topic: " + topicStr);
    }
}


void MQTTService::sendLog(const String& message) {
    String topic = "status/esp32/smartgarden/";    
    // Publish the message to the specified topic
    mqttClient.publish(topic.c_str(), message.c_str());
}