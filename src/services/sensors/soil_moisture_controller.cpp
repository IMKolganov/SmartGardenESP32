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
SoilMoistureStatus SoilMoistureController::handleControlMessage(String message) {
    SoilMoistureStatus status;
    
    int sensorValue = analogRead(sensorPin); // Read the sensor value
    Serial.print("Soil moisture sensor value: ");
    Serial.println(sensorValue);

    // Convert the sensor value to percentage or any other meaningful unit
    // Assuming the sensor value is between 0 and 1023 (typical for analog sensors)
    status.moistureLevel = map(sensorValue, 0, 1023, 0, 100); // Convert to percentage

    // Print to Serial for debugging
    Serial.print("Moisture Level: ");
    Serial.print(status.moistureLevel);
    Serial.println("%");

    // Set success status
    status.success = true;

    return status;
}
