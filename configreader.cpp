#include <iostream> // Temporary

#include "configreader.h"
#include "trim.h"
#include "errorcode.h"

// TODO: Cross-platform method of specifying config location
std::string ConfigReader::CONFIG_FILE { "vinc.conf" };

ConfigReader::ConfigReader()
{
    
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

                ErrorCode status = options.set_option(opt, val);
                if (status != ErrorCode::OK)
                    error(status, opt, val);
            }
        }
    }

    config.close();

    return true;
}

void ConfigReader::error(ErrorCode err, const std::string& opt, const std::string& val)
{
    // TODO: Cross platform method of notifying user of invalid option or value.
    switch (err)
    {
        case ErrorCode::OPT:
        {
            std::cout << "Invalid option found: " << opt << std::endl;
        } break;

        case ErrorCode::VALUE:
        {
            std::cout << "Invalid Value '" << val << "' being assigned to option '" << opt << "'." << std::endl;
        } break;

        case ErrorCode::OK:
        {
            // Can't occur
        } break;
    }
}
