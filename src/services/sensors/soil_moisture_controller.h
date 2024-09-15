#ifndef SOIL_MOISTURE_CONTROLLER_H
#define SOIL_MOISTURE_CONTROLLER_H

#include <Arduino.h>
#include "configuries/config.h"
#include "structs/soil_moisture_status.h"

class SoilMoistureController {
public:
    // Constructor
    SoilMoistureController();

    // Methods for dht control
    void setupSoilMoisture(Config *config); // Initialize the Soil Moisture

    // Method to handle MQTT control messages
    SoilMoistureStatus handleControlMessage(String message);

private:
    int sensorPin;
};

#endif // SOIL_MOISTURE_CONTROLLER_H
