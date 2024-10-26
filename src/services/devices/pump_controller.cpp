#include "pump_controller.h"
#include "configuries/config.h"
#include "structs/pump_status.h"
#include "services/mqtt/mqtt_setup.h"

PumpController::PumpController() {}

void PumpController::setupPump(Config *config) {
    setPumps(config->pumps, 2); //todo: need move 2
    maxPumpDuration = config->pumpMaxPumpDuration;
}

void PumpController::setPumps(Pump* configPumps, int numPumps) {
    for (int i = 0; i < numPumps; i++) {
        pumps[i] = configPumps[i];
        pinMode(pumps[i].pin, OUTPUT);
        digitalWrite(pumps[i].pin, LOW); // Pump is off by default
    }
}

bool PumpController::startPump(int pumpId, int minInterval) {
    unsigned long currentTime = millis();
    Pump &pump = pumps[pumpId];
    
    Serial.print("Pump lastStartTime: ");
    Serial.println(pump.lastStartTime);
    
    if (!pump.isRunning && (currentTime - pump.lastStartTime >= minInterval)) {
        digitalWrite(pump.pin, HIGH);
        pump.lastStartTime = currentTime;
        pump.isRunning = true;
        Serial.println("Pump started successfully.");
        Serial.print("Pump pin: ");
        Serial.println(pump.pin);
        Serial.print("Start time: ");
        Serial.println(currentTime);
        return true;
    } else {
        if (pump.isRunning) {
            Serial.println("Pump is already running.");
        } else {
            Serial.println("Pump cannot be started due to minimum interval restriction.");
            Serial.print("Time since last start: ");
            Serial.println(currentTime - pump.lastStartTime);
            Serial.print("Minimum interval: ");
            Serial.println(minInterval);
        }
    }
    return false;
}

void PumpController::stopPump(int pumpId) {
    Pump &pump = pumps[pumpId];
    digitalWrite(pump.pin, LOW); // todo: fix
    pump.isRunning = false;
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
    if (pump.isRunning && (currentTime - pump.lastStartTime >= maxPumpDuration)) {
        stopPump(pumpId);
        Serial.print("currentTime: ");
        Serial.print(currentTime);
        Serial.print(" lastStartTime: ");
        Serial.print(pump.lastStartTime);
        Serial.print(" maxPumpDuration: ");
        Serial.println(maxPumpDuration);
        Serial.println("Pump stopped automatically due to max duration.");
        mqttServiceInstance.sendLog("Pump stopped automatically due to max duration.");
    }
}

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

    Serial.print("Requested pump ID: ");
    Serial.println(status.pumpId);

    if (status.pumpId >= 0 && status.pumpId < 2) {
        if (startPump(status.pumpId, 0)) { //todo: get min interval from config?
            Serial.print("Pump ");
            Serial.print(status.pumpId);
            Serial.println(" started successfully.");
            status.success = true;
            status.message = "Pump started";
        } else {
            Serial.print("Pump ");
            Serial.print(status.pumpId);
            Serial.println(" could not be started.");
            status.success = false;
            status.message = "Pump cannot be started now";
        }
    } else {
        Serial.println("Invalid pump ID received.");
        status.success = false;
        status.message = "Invalid pump ID";
    }

    return status;
}