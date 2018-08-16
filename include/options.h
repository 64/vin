#ifndef OPTIONS_H
#define OPTIONS_H

#include <unordered_map>
#include <string>

#include "errorcode.h"

class Options
{
public:
    Options();
    ErrorCode set_option(std::string& opt, std::string& val);

private:
    enum class ValueType
    {
        INT,
        BOOL,
        COLOR,
        STRING,
    };

private:
    std::unordered_map<std::string, std::pair<ValueType, std::string>> value_map;
};

#endif // OPTIONS_H
