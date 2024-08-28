#include "pump_control.h"

void setupPumps(Pump* pumps, int numPumps) {
    for (int i = 0; i < numPumps; i++) {
        pinMode(pumps[i].pin, OUTPUT);
        digitalWrite(pumps[i].pin, LOW); // Pumps are off by default
        pumps[i].lastStartTime = 0;
        pumps[i].isRunning = false;
    }
}

bool startPump(Pump* pump) {
    unsigned long currentTime = millis();
    
    // Check if the pump can be started (based on interval and status)
    if (!pump->isRunning && (currentTime - pump->lastStartTime >= minInterval)) {
        digitalWrite(pump->pin, HIGH); // Turn on the pump
        pump->lastStartTime = currentTime;
        pump->isRunning = true;
        return true;
    }
    return false;
}

void stopPump(Pump* pump) {
    digitalWrite(pump->pin, LOW); // Turn off the pump
    pump->isRunning = false;
}

void updatePump(Pump* pump) {
    unsigned long currentTime = millis();
    
    // Check if the pump has been running longer than maxPumpDuration
    if (pump->isRunning && (currentTime - pump->lastStartTime >= maxPumpDuration)) {
        stopPump(pump);
    }
}
