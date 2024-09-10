#include <Arduino.h>
#include "sensors.h"
#include "pump_control.h"

#define DHTPIN 4           // Pin connected to the DHT22
#define DHTTYPE DHT11      // Type of DHT sensor
#define SOIL_SENSOR_PIN 34 // Pin connected to the soil moisture sensor

DHT dht(DHTPIN, DHTTYPE);
Pump* pumps_g = nullptr;  // Define the global pumps_g variable here

void setupSensors(Config *config) {
  dht.begin();

  pumps_g = new Pump[2];  // Allocate memory for the pumps
  Serial.println("Started setup pumps...");
  for (int i = 0; i < 2; i++) {
      pumps_g[i] = config->pumps[i];  // Load pumps from config
      Serial.printf("Pump %d: Pin=%d, Last Start Time=%lu, Is Running=%s\n", 
                    i + 1, pumps_g[i].pin, pumps_g[i].lastStartTime, pumps_g[i].isRunning ? "true" : "false");
  }
  
  setupPumps(pumps_g, 2);  // Initialize pumps
}

  // float temperature = dht.readTemperature();
  // float humidity = dht.readHumidity();


  //need think about code:
    //   for (int i = 0; i < 2; i++) {
    //     updatePump(&pumps[i]);
    // }