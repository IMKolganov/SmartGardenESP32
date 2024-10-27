// udp_module.h
#ifndef UDP_MODULE_H
#define UDP_MODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>

void initializeUDPBroadcastModule(const int udpPort, const int delayTimeMs, const String taskName = "UDP Task", const unsigned long taskStackSize = 10000);

void udpTask(void *pvParameters);

#endif // UDP_MODULE_H
