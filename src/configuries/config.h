#ifndef CONFIG_H
#define CONFIG_H

#include "struct/config_struct.h"

void loadConfig();
Config readFromConfigFile(const char* filename);

#endif // CONFIG_H