#ifndef SOIL_MOISTURE_STATUS_H
#define SOIL_MOISTURE_STATUS_H

#include <Arduino.h>

struct SoilMoistureStatus {
    bool success;
    int moistureLevel; // Moisture level in percentage

    String toJson() const {
        JsonDocument doc;

        // Populate the JsonDocument
        doc["success"] = success;
        doc["moistureLevel"] = moistureLevel;

        // Serialize JsonDocument to a String
        String jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }
};

#endif // SOIL_MOISTURE_STATUS_H
