// udp_module.cpp
#include "udp_module.h"
#include "services/mqtt/mqtt_setup.h"

WiFiUDP udp;

struct UDPParams {
    String taskName;
    int udpPort;
    int delayTimeMs;
    unsigned long taskStackSize;
};

IPAddress calculateBroadcastIP() {
    IPAddress localIP = WiFi.localIP();
    IPAddress subnetMask = WiFi.subnetMask();

    IPAddress broadcastIP;
    for (int i = 0; i < 4; i++) {
        broadcastIP[i] = localIP[i] | ~subnetMask[i];
    }
    return broadcastIP;
}

void initializeUDPBroadcastModule(const int udpPort, const int delayTimeMs, const String taskName, const unsigned long taskStackSize) {
    Serial.println("Booting");

    UDPParams *udpParams = new UDPParams;
    udpParams->taskName = taskName;
    udpParams->udpPort = udpPort;
    udpParams->delayTimeMs = delayTimeMs;
    udpParams->taskStackSize = taskStackSize;

    // Create a task for UDP Broadcast
    xTaskCreatePinnedToCore(
        udpTask,    // Pointer to the task function
        udpParams->taskName.c_str(), // Task name
        udpParams->taskStackSize,      // Task stack size
        udpParams,  // Parameters for the task function
        1,          // Task priority
        NULL,       // Task handle
        1           // Processor core (0 or 1)
    );
}

void udpTask(void *pvParameters) {
    UDPParams *params = (UDPParams *)pvParameters;

    while (true) {
        IPAddress ip = WiFi.localIP();
        String ipString = ip.toString();

        IPAddress broadcastIP = calculateBroadcastIP();
        udp.beginPacket(broadcastIP, params->udpPort);
        udp.print(ipString);
        udp.endPacket();

        Serial.println("Sent IP address via Broadcast: " + ipString);
        mqttServiceInstance.sendLog("Sent IP address via Broadcast: " + ipString);

        vTaskDelay(params->delayTimeMs / portTICK_PERIOD_MS);
    }
}
