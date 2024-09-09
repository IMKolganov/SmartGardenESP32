#ifndef CONFIG_H
#define CONFIG_H

struct Config {
  String ssid;
  String password;
  String device_key;
  String device_name;
  int udpPort;
  int udpDelayTimeMs;
};


void loadConfig();
Config readFromConfigFile(const char* filename);

#endif // CONFIG_H