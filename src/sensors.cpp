#include <Arduino.h>
#include "sensors.h"
#include "pump_control.h"
#include "pump_struct.h"

#define DHTPIN 4           // Pin connected to the DHT22
#define DHTTYPE DHT11      // Type of DHT sensor
#define SOIL_SENSOR_PIN 34 // Pin connected to the soil moisture sensor

DHT dht(DHTPIN, DHTTYPE);
Pump *pumps;
// extern Pump pumps[];

void setupSensors(Config *config) {
  dht.begin();

  pumps = new Pump[2];
  for (int i = 0; i < 2; i++) {
      pumps[i] = config->pumps[i];
      Serial.printf("Pump %d: Pin=%d, Last Start Time=%lu, Is Running=%s\n", 
                    i + 1, pumps[i].pin, pumps[i].lastStartTime, pumps[i].isRunning ? "true" : "false");
  }
  setupPumps(pumps, 2);
}

  // float temperature = dht.readTemperature();
  // float humidity = dht.readHumidity();


  //need think about code:
    //   for (int i = 0; i < 2; i++) {
    //     updatePump(&pumps[i]);
    // }