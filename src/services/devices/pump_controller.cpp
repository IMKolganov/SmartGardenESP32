#include "pump_controller.h"
#include "configuries/config.h"
#include "structs/pump_status.h"
#include "services/mqtt/mqtt_setup.h"

PumpController::PumpController() {}

void PumpController::setupPump(Config *config) {
    setPumps(config->pumps, 2); //todo: need move 2
    minIntervalDefault = config->minIntervalDefault;
    pumpDurationDefault = config->pumpDurationDefault;
}

void PumpController::setPumps(Pump* configPumps, int numPumps) {
    for (int i = 0; i < numPumps; i++) {
        pumps[i] = configPumps[i];
        pinMode(pumps[i].pin, OUTPUT);
        digitalWrite(pumps[i].pin, LOW); // Pump is off by default
    }
}

bool PumpController::startPump(int pumpId, int minInterval, unsigned long duration) {
    unsigned long currentTime = millis();
    Pump &pump = pumps[pumpId];
    
    if (!pump.isRunning && (currentTime - pump.lastStartTime >= minInterval)) {
        digitalWrite(pump.pin, HIGH);
        pump.lastStartTime = currentTime;
        pump.startTime = currentTime;
        pump.runDuration = duration;
        pump.isRunning = true;
        
        Serial.println("Pump started successfully.");
        return true;
    } else {
        if (pump.isRunning) {
            unsigned long timeElapsed = currentTime - pump.startTime;
            unsigned long timeRemaining = (timeElapsed < pump.runDuration) ? (pump.runDuration - timeElapsed) : 0;
            
            Serial.println("Pump is already running.");
            Serial.print("Time elapsed: ");
            Serial.println(timeElapsed);
            Serial.print("Time remaining: ");
            Serial.println(timeRemaining);
        } else {
            Serial.println("Pump cannot be started due to minimum interval restriction.");
        }
    }
    return false;
}


void PumpController::stopPump(int pumpId) {
    Pump &pump = pumps[pumpId];
    digitalWrite(pump.pin, LOW); // todo: fix
    pump.isRunning = false;
    pump.lastStartTime = millis();
    Serial.println("Pump stopped.");
}

void PumpController::updateAllPumps() {
      for (int i = 0; i < 2; i++) {//todo: need move 2
        updatePump(i);
    }
}

void PumpController::updatePump(int pumpId) {
    unsigned long currentTime = millis();
    Pump &pump = pumps[pumpId];
    
    if (pump.isRunning) {
        unsigned long timeElapsed = currentTime - pump.startTime;

        if (timeElapsed >= pump.runDuration) {
            stopPump(pumpId);
            Serial.print("Pump ");
            Serial.print(pumpId);
            Serial.println(" stopped automatically due to duration expiration.");
            mqttServiceInstance.sendLog("Pump " + String(pumpId) + " stopped automatically due to duration expiration.");
        } else {
            unsigned long timeRemaining = pump.runDuration - timeElapsed;
            // Serial.print("Pump ");
            // Serial.print(pumpId);
            // Serial.print(" is running. Time elapsed: ");
            // Serial.print(timeElapsed);
            // Serial.print(" ms, Time remaining: ");
            // Serial.print(timeRemaining);
            // Serial.println(" ms.");
        }
    }
}


// Handle control message (returns the sensor status)
PumpStatus PumpController::handleControlMessage(String message) {
    PumpStatus status;
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);
    
    if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        status.message = error.c_str();
        status.success = false;
        return status;
    }
    
    if (doc.containsKey("RequestId")) {
        status.requestId = doc["RequestId"].as<String>();
    } else {
        Serial.println("RequestId not found in message");
        status.message = "RequestId not found in message";
        status.success = false;
        return status;
    }
    
    if (doc.containsKey("PumpId")) {
        status.pumpId = doc["PumpId"].as<int>();
    } else {
        Serial.println("PumpId not found in message");
        status.message = "PumpId not found in message";
        status.success = false;
        return status;
    }
    
    unsigned long duration = doc.containsKey("Duration") ? doc["Duration"].as<unsigned long>() : pumpDurationDefault;
    
    Serial.print("Requested pump ID: ");
    Serial.println(status.pumpId);
    
    int minInterval = minIntervalDefault;

    if (status.pumpId >= 0 && status.pumpId < 2) {
        unsigned long currentTime = millis();
        Pump &pump = pumps[status.pumpId];
        
        if (!pump.isRunning && (currentTime - pump.lastStartTime >= minInterval)) {
            startPump(status.pumpId, minInterval, duration);
            status.success = true;
            status.message = "Pump started";
            status.timeElapsed = 0;
            status.timeRemaining = duration;
            status.minInterval = minInterval;
            
            Serial.print("Pump ");
            Serial.print(status.pumpId);
            Serial.println(" started successfully.");
        } else {
            status.success = false;
            unsigned long timeElapsed = pump.isRunning ? (currentTime - pump.startTime) : 0;
            unsigned long timeRemaining = pump.isRunning ? (pump.runDuration - timeElapsed) : 0;
            unsigned long timeSinceLastStart = currentTime - pump.lastStartTime;
            
            if (pump.isRunning) {
                status.message = "Pump is already running";
                status.timeElapsed = timeElapsed;
                status.timeRemaining = timeRemaining;
            } else {
                status.message = "Pump cannot be started now due to min interval restriction";
                status.timeElapsed = timeSinceLastStart;
                status.timeRemaining = minInterval - timeSinceLastStart;
            }
            
            status.minInterval = minInterval;
            
            Serial.print("Pump ");
            Serial.print(status.pumpId);
            Serial.println(" could not be started.");
            Serial.print("Time elapsed: ");
            Serial.println(status.timeElapsed);
            Serial.print("Time remaining: ");
            Serial.println(status.timeRemaining);
            Serial.print("Minimum interval: ");
            Serial.println(status.minInterval);
        }
    } else {
        Serial.println("Invalid pump ID received.");
        status.success = false;
        status.message = "Invalid pump ID";
    }
    
    return status;
}