// udp_module.cpp
#include "udp_module.h"

WiFiUDP udp;

struct UDPParams {
    int udpPort;
    int delayTimeMs;
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

void initializeUDPBroadcastModule(const int udpPort, const int delayTimeMs) {
    Serial.begin(115200);
    Serial.println("Booting");

    UDPParams *udpParams = new UDPParams;
    udpParams->udpPort = udpPort;
    udpParams->delayTimeMs = delayTimeMs;

    // Create a task for UDP Broadcast
    xTaskCreatePinnedToCore(
        udpTask,    // Pointer to the task function
        "UDP Task", // Task name
        10000,      // Task stack size
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

        vTaskDelay(params->delayTimeMs / portTICK_PERIOD_MS);
    }
}
