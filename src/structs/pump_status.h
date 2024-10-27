#ifndef PUMP_STATUS_H
#define PUMP_STATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct PumpStatus {
    String requestId;
    int pumpId;
    bool success;
    String message;

    String toJson() const {
        JsonDocument doc;
        doc["RequestId"] = requestId;
        doc["PumpId"] = pumpId;        
        doc["Success"] = success;        
        doc["Message"] = message;

        // Serialize JsonDocument to a String
        String jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }
};

#endif // PUMP_STATUS_H
