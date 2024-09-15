#ifndef DHT_STATUS_H
#define DHT_STATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct DhtStatus {
    bool success;
    float temperature;
    float humidity;
    String errorMessage;

    String toJson() const {
        JsonDocument doc;

        // Populate the JsonDocument
        doc["success"] = success;
        doc["errorMessage"] = errorMessage;
        doc["temperature"] = temperature;
        doc["humidity"] = humidity;

        // Serialize JsonDocument to a String
        String jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }
};

#endif // DHT_STATUS_H
