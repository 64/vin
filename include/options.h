#ifndef OPTIONS_H
#define OPTIONS_H

#include <unordered_map>
#include <string>

#include "errorcode.h"

class Options
{
private:
    enum class ValueType
    {
        INT,
        BOOL,
        COLOR,
        STRING,
    };

public:
    Options();
    ErrorCode set_option(const std::string& opt, std::string& val);
    bool validate(ValueType val_type, std::string& val);

private:
    std::unordered_map<std::string, std::pair<ValueType, std::string>> value_map;
};

#endif // OPTIONS_H
