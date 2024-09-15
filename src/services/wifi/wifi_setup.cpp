#include <WiFi.h>
#include "wifi_setup.h"

void setupWiFi(const char* ssid, const char* password) {
  if (ssid == nullptr || strlen(ssid) == 0) {
    Serial.println("Error: SSID is empty");
    return;
  }
  if (password == nullptr || strlen(password) == 0) {
    Serial.println("Error: Password is empty");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    
    Serial.print("Connecting to WiFi... SSID: ");
    Serial.println(ssid);
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
