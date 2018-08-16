#include <algorithm>
#include <cctype>

#include "options.h"

Options::Options()
{
    // Set defaults
    value_map =
    {
        {"bg_color", {ValueType::COLOR, "0x4A6F6F"}},
    };
}

ErrorCode Options::set_option(std::string& opt, std::string& val)
{
    if (value_map.find(opt) == value_map.end())
        return ErrorCode::OPT;

    switch (value_map[opt].first)
    {
        case ValueType::INT:
        {
            try { std::stoi(val); }
            catch (std::invalid_argument&) { return ErrorCode::VALUE; }
        } break;

        case ValueType::COLOR:
        {
            try
            {
                int col = std::stoi(val);
                if (col >= 0x000000 && col <= 0xFFFFFF)
                    return ErrorCode::OK;
            }
            catch (std::invalid_argument&) { return ErrorCode::VALUE; }
        } break;

        case ValueType::BOOL:
        {
            std::transform(val.begin(), val.end(), val.begin(), tolower);
            if (val != "true" && val != "false")
                return ErrorCode::VALUE;
        } break;

        case ValueType::STRING:
        {
            // Possibly check validity against option
            return ErrorCode::OK;
        } break;
    }

    return ErrorCode::OK;
}
