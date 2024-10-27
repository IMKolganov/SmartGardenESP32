#ifndef SOIL_MOISTURE_STATUS_H
#define SOIL_MOISTURE_STATUS_H

#include <Arduino.h>

struct SoilMoistureStatus {
    String requestId;
    int sensorId;
    String message;
    bool success;
    int moistureLevel;

    String toJson() const {
        JsonDocument doc;

        // Populate the JsonDocument
        doc["RequestId"] = requestId;
        doc["SensorId"] = sensorId;
        doc["Message"] = message;
        doc["Success"] = success;
        doc["MoistureLevel"] = moistureLevel;

        // Serialize JsonDocument to a String
        String jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }
};

#endif // SOIL_MOISTURE_STATUS_H
