#ifndef PUMP_STATUS_H
#define PUMP_STATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct PumpStatus {
    String requestId;
    int pumpId;
    bool success;
    String message;
    unsigned long timeElapsed; // Time elapsed since startup
    unsigned long timeRemaining; // Remaining running time
    unsigned long minInterval; // Minimum interval between startups

    String toJson() const {
        JsonDocument doc;
        doc["RequestId"] = requestId;
        doc["PumpId"] = pumpId;        
        doc["Success"] = success;        
        doc["Message"] = message;
        doc["TimeElapsed"] = timeElapsed;
        doc["TimeRemaining"] = timeRemaining;
        doc["MinInterval"] = minInterval;

        // Serialize JsonDocument to a String
        String jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }
};

#endif // PUMP_STATUS_H
