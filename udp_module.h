// udp_module.h
#ifndef UDP_MODULE_H
#define UDP_MODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>

void initializeUDPBroadcastModule();

void udpTask(void *pvParameters);

#endif // UDP_MODULE_H
