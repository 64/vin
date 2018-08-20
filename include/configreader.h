#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>
#include <fstream>

#include "options.h"
#include "errorcode.h"

class ConfigReader
{
public:
    ConfigReader();
    bool load();
    template<typename T> T option(const std::string& opt);
    [[noreturn]] void error(ErrorCode err, const std::string& opt, const std::string& val);

private:
    static std::string CONFIG_FILE;
    Options options;

};

template<> std::string ConfigReader::option<std::string>(const std::string& opt);
template<> bool ConfigReader::option<bool>(const std::string& opt);
template<> int ConfigReader::option<int>(const std::string& opt);

#endif // CONFIGREADER_H
