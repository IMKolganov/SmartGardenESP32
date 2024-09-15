#ifndef DHT_CONTROLLER_H
#define DHT_CONTROLLER_H

#include <Arduino.h>
#include <DHT.h>
#include "configuries/config.h"
#include "structs/dht_status.h"

class DhtController {
public:
    // Constructor
    DhtController();

    // Methods for dht control
    void setupDht(Config *config); // Initialize the dht

    // Method to handle MQTT control messages
    DhtStatus handleControlMessage(String message);

private:
    DHT *dht;
    int sensorPin;
    int dhtType;
};

#endif // DHT_CONTROLLER_H
