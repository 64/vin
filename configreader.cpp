#include "configreader.h"

std::string ConfigReader::CONFIG_FILE { "vinc.conf" };

ConfigReader::ConfigReader()
{
    
}

bool ConfigReader::load()
{
    std::ifstream config { CONFIG_FILE };
    
    std::string line;
    while (std::getline(config, line))
    {
        continue;
    }

    return true;
}
