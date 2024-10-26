#ifndef DHT_STATUS_H
#define DHT_STATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct DhtStatus {
    String requestId;
    int sensorId;
    bool success;
    String message;
    float temperature;
    float humidity;    

    String toJson() const {
        JsonDocument doc;

        // Populate the JsonDocument
        doc["RequestId"] = requestId;
        doc["SensorId"] = sensorId;
        doc["Success"] = success;
        doc["Message"] = message;
        doc["Temperature"] = temperature;
        doc["Humidity"] = humidity;

        // Serialize JsonDocument to a String
        String jsonString;
        serializeJson(doc, jsonString);
        return jsonString;
    }
};

#endif // DHT_STATUS_H
