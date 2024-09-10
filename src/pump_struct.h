#ifndef PUMP_STRUCT_H
#define PUMP_STRUCT_H

#include <Arduino.h>

// Structure to represent a pump
struct Pump {
    int pin;                      // Pin connected to the relay
    unsigned long lastStartTime;  // Last start time
    bool isRunning;               // Pump status (running or not)
};

#endif // PUMP_STRUCT_H
