#include <iostream> // Temporary
#include <utility>
#include <cstdlib>
#include <string>

#include "configreader.h"
#include "trim.h"
#include "errorcode.h"
#include "util.h"

// TODO: Cross-platform method of specifying config location
std::string ConfigReader::CONFIG_FILE { "vin.conf" };

ConfigReader::ConfigReader()
{
    
}

template <>
std::string ConfigReader::option<std::string>(const std::string& opt)
{
    return options.get_value(opt);
}

template <>
bool ConfigReader::option<bool>(const std::string& opt)
{
    return (options.get_value(opt) == "true" || options.get_value(opt) == "TRUE");
}

template <>
int ConfigReader::option<int>(const std::string& opt)
{
    std::string value = options.get_value(opt);
    if (value.size() > 2 && value[1] == 'x')
        return std::stoi(value, nullptr, 16);
    else
        return std::stoi(value);
}

bool ConfigReader::load()
{
    std::ifstream config { CONFIG_FILE, std::ios_base::in };
    
    if (config)
    {
        std::string line;
        while (std::getline(config, line))
        {
#ifdef __DEBUG__
            if (!line.empty())
                std::cout << "Read option '" << line << "'" << std::endl;
#endif
            auto eq = line.find('=');
            if (eq != std::string::npos)
            {
                std::string opt = line.substr(0, eq);
                std::string val = line.substr(eq + 1);

                ErrorCode status = options.set_option(trim(opt), trim(val));
                if (status != ErrorCode::OK)
                    error(status, opt, val);
#ifdef __DEBUG__
                else
                    std::cout << "Loaded value '" << val << "' into option '" << opt << "'" << std::endl;
#endif
            }
        }
    }

    config.close();

    return true;
}

void ConfigReader::error(ErrorCode err, const std::string& opt, const std::string& val)
{
    switch (err)
    {
        case ErrorCode::OPT:
        {
            message_box("Invalid Option", "Found invalid option '" + opt + "' in config.", true);
        } break;

        case ErrorCode::VALUE:
        {
            message_box("Invalid Value", "Invalid Value '" + val + "' being assigned to option '" + opt + "'.", true);
        } break;

        case ErrorCode::OK:
        {
            // Can't occur
        } break;
    }

    std::exit(EXIT_FAILURE);
}
