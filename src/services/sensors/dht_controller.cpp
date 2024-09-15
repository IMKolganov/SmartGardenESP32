#include <DHT.h>
#include "dht_controller.h"
#include "configuries/config.h"
#include "structs/dht_status.h"

// Constructor
DhtController::DhtController() : dht(nullptr) {}

// DHT initialization
void DhtController::setupDht(Config *config) {
    sensorPin = config->dhtPin;
    dhtType = config->dhtType;
    dht = new DHT(sensorPin, dhtType);
    dht->begin(); // Start the sensor
}

// Handle control message (returns the sensor status)
DhtStatus DhtController::handleControlMessage(String message) {
    DhtStatus status;

    // Read temperature and humidity from the sensor
    float humidity = dht->readHumidity();
    float temperature = dht->readTemperature();

    // Check for errors in reading data
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Error reading data from DHT");
        status.success = false;
        status.temperature = 0;
        status.humidity = 0;
        status.errorMessage = "Error reading data from DHT";
    } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("°C, Humidity: ");
        Serial.print(humidity);
        Serial.println("%");

        // Fill in the status
        status.success = true;
        status.temperature = temperature;
        status.humidity = humidity;
    }

    return status; // Return the sensor status
}
