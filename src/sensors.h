#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include <ESPAsyncWebServer.h>

void setupSensors();
void handleTemperatureHumidityRequest(AsyncWebServerRequest *request);
void handleSoilMoistureRequest(AsyncWebServerRequest *request);
void handleSerialInput();

#endif // SENSORS_H
