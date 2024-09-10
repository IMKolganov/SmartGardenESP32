#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include "config.h"
#include "pump_struct.h"

extern Pump* pumps_g;  // Declare pumps_g as external so it can be accessed in other files

void setupSensors(Config *config);

#endif // SENSORS_H
