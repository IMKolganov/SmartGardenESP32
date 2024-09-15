#ifndef PUMP_STATUS_H
#define PUMP_STATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct PumpStatus {
    bool success;
    String message;

    String toJson() const {
        JsonDocument doc;

        // Populate the JsonDocument
        doc["success"] = success;
        doc["message"] = message;

        // Serialize JsonDocument to a String
        String jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }
};

#endif // PUMP_STATUS_H
