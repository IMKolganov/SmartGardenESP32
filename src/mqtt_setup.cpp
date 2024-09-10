#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_setup.h"
#include "pump_control.h"

// MQTT settings
const char* mqttServer = "192.168.10.18";
const int mqttPort = 1883;
const char* mqttUser = "guest";
const char* mqttPassword = "guest";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

extern Pump pumps[];
// Pump *pumps;

void setupMQTT() {
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(mqttCallback);

    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT...");
        if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
            Serial.println("connected");
            mqttClient.subscribe("pump/control");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Преобразование payload в строку
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    // Вывод информации о полученном сообщении
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message payload: ");
    Serial.println(message);

    // Обработка сообщений на основе топика
    if (String(topic) == "pump/control") {
        int pumpId = message.toInt();
        Serial.print("Requested pump ID: ");
        Serial.println(pumpId);

        if (pumpId >= 0 && pumpId < 2) {
            if (startPump(&pumps[pumpId])) {
                Serial.print("Pump ");
                Serial.print(pumpId);
                Serial.println(" started successfully.");
                mqttClient.publish("pump/status", "Pump started");
            } else {
                Serial.print("Pump ");
                Serial.print(pumpId);
                Serial.println(" could not be started.");
                mqttClient.publish("pump/status", "Pump cannot be started now");
            }
        } else {
            Serial.println("Invalid pump ID received.");
            mqttClient.publish("pump/status", "Invalid pump ID");
        }
    } else {
        Serial.print("Unknown topic: ");
        Serial.println(topic);
    }
}

// void pumpControl()