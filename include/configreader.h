#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>
#include <fstream>

class ConfigReader
{
public:
    ConfigReader();
    bool load();

private:
    static std::string CONFIG_FILE;

};

#endif // CONFIGREADER_H
