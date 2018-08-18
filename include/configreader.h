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
    std::string option(const std::string& opt);
    [[noreturn]] void error(ErrorCode err, const std::string& opt, const std::string& val);

private:
    static std::string CONFIG_FILE;
    Options options;

};

#endif // CONFIGREADER_H
