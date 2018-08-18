#include <algorithm>
#include <cctype>
#include <iostream>
#include <cassert>

#include "options.h"

bool valid_number(const std::string& val)
{
    bool dec { true };
    bool hex { true };

    // Check if valid decimal number
    for (const auto digit : val)
        if (!std::isdigit(digit))
            dec = false;

    // Check if valid hex number
    if (val.size() > 2 && val[0] == '0' && std::tolower(val[1]) == 'x')
        for (const auto digit : val.substr(2))
            if (!std::isxdigit(digit))
                hex = false;

    return (dec || hex);
}

bool valid_color(const std::string& val)
{
    bool valid { true };

    try
    {
        if (valid_number(val))
        {
            int col = std::stoi(val); // Maybe change for RGBA values
            valid = (col >= 0x000000 && col <= 0xFFFFFF) ? true : false;
            std::cout << col << std::endl;
            std::cout << val << std::endl;
        }
        else
        {
            valid = false;
        }
    }
    catch (std::invalid_argument&) { valid = false; }

    return valid;
}

Options::Options()
{
    // Set defaults
    value_map =
    {
        {"bg_color",     {ValueType::COLOR, "0x4A6F6F"}},
        {"line_numbers", {ValueType::BOOL, "FALSE"}},
        {"font_path",    {ValueType::STRING, "/usr/share/fonts/TTF/DejaVuSansMono.ttf"}},
        {"font_size",    {ValueType::INT, "14"}}
    };
}

std::string Options::get_value(const std::string& opt)
{
    assert(value_map.find(opt) != value_map.end());
    return value_map[opt].second;
}

bool Options::validate(ValueType val_type, const std::string& val)
{
    bool status = true;
    switch (val_type)
    {
        case ValueType::INT:
        {
            status = valid_number(val);
        } break;

        case ValueType::COLOR:
        {
            status = valid_color(val);
        } break;

        case ValueType::BOOL:
        {
            status = (val == "true" || val == "false" || val == "TRUE" || val == "FALSE") ? true : false;
        } break;

        case ValueType::STRING:
        {
            status = true; // No current way of checking a valid string value
        } break;
    }

    return status;
}

ErrorCode Options::set_option(const std::string& opt, const std::string& val)
{
    // Check Invalid Option
    if (value_map.find(opt) == value_map.end())
        return ErrorCode::OPT;

    // Check Invalid Value
    if (!validate(value_map[opt].first, val))
        return ErrorCode::VALUE;

    // Set the option
    value_map[opt].second = val;

    return ErrorCode::OK;
}
