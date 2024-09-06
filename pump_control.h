#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H

#include <Arduino.h>

// Constants
const unsigned long maxPumpDuration = 30000; // Maximum pump run time (30 seconds)
const unsigned long minInterval = 60000;    // Minimum interval between runs (600000 = 10 minutes)

// Structure to represent a pump
struct Pump {
    int pin;                      // Pin connected to the relay
    unsigned long lastStartTime;  // Last start time
    bool isRunning;               // Pump status (running or not)
};

// Function prototypes
void setupPumps(Pump* pumps, int numPumps);
bool startPump(Pump* pump);
void stopPump(Pump* pump);
void updatePump(Pump* pump);

#endif // PUMP_CONTROL_H
