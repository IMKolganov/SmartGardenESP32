#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H

#include <Arduino.h>
#include <configuries/config.h>
#include <structs/pump_status.h>

class PumpController {
public:
    // Constructor
    PumpController();

    // Methods for pump control
    void setupPump(Config *config); // Initialize the pump
    bool startPump(int pumpId, int minInterval); // Start the pump with a minimum interval
    void stopPump(int pumpId); // Stop the pump
    void updatePump(int pumpId); // Update the pump state based on duration
    void updateAllPumps();

    // Method to handle MQTT control messages
    PumpStatus handleControlMessage(int pumpId, String message);

    // Getter methods
    bool isRunning() const;
    unsigned long getLastStartTime() const;

    void setPumps(Pump* pumps, int numPumps);

private:
    Pump pumps[2];

    int maxPumpDuration;
};

#endif // PUMP_CONTROL_H
