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

    if (!pump->isRunning && (currentTime - pump->lastStartTime >= minInterval)) {
        digitalWrite(pump->pin, HIGH);
        pump->lastStartTime = currentTime;
        pump->isRunning = true;
        Serial.println("Pump started successfully.");
        Serial.print("Pump pin: ");
        Serial.println(pump->pin);
        Serial.print("Start time: ");
        Serial.println(currentTime);
        return true;
    } else {
        if (pump->isRunning) {
            Serial.println("Pump is already running.");
        } else {
            Serial.println("Pump cannot be started due to minimum interval restriction.");
            Serial.print("Time since last start: ");
            Serial.println(currentTime - pump->lastStartTime);
            Serial.print("Minimum interval: ");
            Serial.println(minInterval);
        }
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
