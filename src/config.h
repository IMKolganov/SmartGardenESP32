#ifndef CONFIG_H
#define CONFIG_H

#include "config_struct.h"

void loadConfig();
Config readFromConfigFile(const char* filename);

#endif // CONFIG_H