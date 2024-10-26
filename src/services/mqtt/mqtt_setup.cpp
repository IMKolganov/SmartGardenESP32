#include "mqtt_setup.h"

// Initialize the global instance
MQTTService mqttServiceInstance;

MQTTService::MQTTService() : mqttClient(espClient), pumpController(), dhtController(), soilMoistureController(), config(nullptr) {
}

void MQTTService::setupMQTT(Config *config) {
    this->config = config;  // Store the pointer to the Config object
    mqttClient.setServer(config->mqttServer.c_str(), config->mqttPort);
    mqttClient.setCallback(mqttCallback);

    pumpController.setupPump(config);
    dhtController.setupDht(config);
    soilMoistureController.setupSoilMoisture(config);

    if (connectToMQTT()) {
        Serial.println("Connected to MQTT");
    } else {
        Serial.println("Failed to connect to MQTT");
        delay(10000);
        ESP.restart();
    }
}

bool MQTTService::connectToMQTT() {
    if (config == nullptr) {
        Serial.println("Config is not set!");
        return false;
    }

    Serial.print("Connecting to MQTT... IP: ");
    Serial.println(config->mqttServer);
    Serial.print(" Port: ");
    Serial.println(config->mqttPort);

    if (mqttClient.connect("ESP32Client", config->mqttUser.c_str(), config->mqttPassword.c_str())) {
        Serial.println("Connected");
        mqttClient.subscribe("control/pump/#");
        mqttClient.subscribe("control/dht/");
        mqttClient.subscribe("control/soil-moisture/");
        // mqttClient.subscribe("status/esp32/smartgarden/");
        return true;
    } else {
        Serial.print("Failed, rc=");
        Serial.print(mqttClient.state());
        return false;
    }
}

void MQTTService::loop() {
    if (!mqttClient.connected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > 5000) { // Attempt to reconnect every 5 seconds
            lastReconnectAttempt = now;
            if (connectToMQTT()) {
                lastReconnectAttempt = 0; // Reset the reconnect attempt time
            }
        }
    } else {
        mqttClient.loop();
    }
}

void MQTTService::sendMessage(const String& topic, const String& message) {
//    Serial.println("Sent message to: " + String(topic) + " message: " + String(message));
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
        statusTopic = "status/pump/";
        PumpStatus pumpStatus = pumpController.handleControlMessage(message);
        jsonResponse = pumpStatus.toJson();
        sendMessage(statusTopic, jsonResponse);
    } 
    else if (topicStr.startsWith("control/dht/")) {
        DhtStatus dhtStatus = dhtController.handleControlMessage(message);
        statusTopic = "status/dht/";
        jsonResponse = dhtStatus.toJson();
        sendMessage(statusTopic, jsonResponse);
    }
    else if (topicStr.startsWith("control/soil-moisture/")) {
        SoilMoistureStatus soilMoistureStatus = soilMoistureController.handleControlMessage(message);
        statusTopic = "status/soil-moisture/";
        jsonResponse = soilMoistureStatus.toJson();
        sendMessage(statusTopic, jsonResponse);
    }
    else {
        Serial.println("Unknown topic: " + topicStr);
    }
}

void MQTTService::sendLog(const String& message) {
    String topic = "status/esp32/smartgarden/";    
    sendMessage(topic, message);
}
