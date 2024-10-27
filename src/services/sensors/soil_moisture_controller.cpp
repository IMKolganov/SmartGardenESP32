#include "soil_moisture_controller.h"
#include "configuries/config.h"
#include "structs/soil_moisture_status.h"

// Constructor
SoilMoistureController::SoilMoistureController() {}

// SoilMoisture initialization
void SoilMoistureController::setupSoilMoisture(Config *config) {
    sensorPin = config->soilSensorPin;
    pinMode(sensorPin, INPUT);
}

// Handle control message (returns the sensor status)
#include <ArduinoJson.h>

SoilMoistureStatus SoilMoistureController::handleControlMessage(String message) {
    SoilMoistureStatus status;

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        status.message = error.c_str();
        status.success = false;
        return status;
    }

    if (doc.containsKey("RequestId")) {
        status.requestId = doc["RequestId"].as<String>();
    } else {
        Serial.println("RequestId not found in message");
        status.message = "RequestId not found in message";
        status.success = false;
        return status;
    }

    if (doc.containsKey("SensorId")) {
        status.sensorId = doc["SensorId"].as<int>();
    }

    int sensorValue = analogRead(sensorPin);
    Serial.print("Soil moisture sensor value: ");
    Serial.println(sensorValue);

    status.moistureLevel = map(sensorValue, 0, 1023, 0, 100);
    Serial.print("Moisture Level: ");
    Serial.print(status.moistureLevel);
    Serial.println("%");

    status.success = true;

    return status;
}