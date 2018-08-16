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

bool Options::validate(ValueType val_type, std::string& val)
{
    bool status = true;
    switch (val_type)
    {
        case ValueType::INT:
        {
            bool dec { true };
            bool hex { true };

            for (const auto digit : val)
                if (!std::isdigit(digit))
                    dec = false;

            if (val.size() > 2 && val[0] == '0' && std::tolower(val[1]) == 'x')
                for (const auto digit : val.substr(2))
                    if (!std::isxdigit(digit))
                        hex = false;

            status = (dec || hex) ? true : false;
        } break;

        case ValueType::COLOR:
        {
            try
            {
                // Potentially change in future for RGBA values
                int col = std::stoi(val);
                status = (col >= 0x000000 && col <= 0xFFFFFF) ? true : false;
            }
            catch (std::invalid_argument&) { status = false; }
        } break;

        case ValueType::BOOL:
        {
            std::transform(val.begin(), val.end(), val.begin(), ::tolower);
            if (val != "true" && val != "false")
                status = false;
        } break;
    }

    return status;
}

ErrorCode Options::set_option(const std::string& opt, const std::string& val)
{
    if (value_map.find(opt) == value_map.end())
        return ErrorCode::OPT;

    if (!validate(value_map[opt].first, value_map[opt].second))
        return ErrorCode::VALUE;

    value_map[opt].second = val;

    return ErrorCode::OK;
}
