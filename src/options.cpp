#include <algorithm>
#include <cctype>
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
            valid = (col >= 0x000000 && col <= 0xFFFFFF);
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
        {"bg_color",     {ValueType::COLOR,  "0x002B35"}},  // Background Color
        {"fg_color",     {ValueType::COLOR,  "0x829496"}},  // Text Color
        {"cl_color",     {ValueType::COLOR,  "0x023641"}},  // Color used to highlight current line
        {"cr_color",     {ValueType::COLOR,  "0x92A1A1"}},  // Caret Color
        {"ln_color",     {ValueType::COLOR,  "0x576E75"}},  // Line numbers Color
        {"gt_color",     {ValueType::COLOR,  "0x023641"}},  // Gutter Color
        {"font_path",    {ValueType::STRING, "/usr/share/fonts/TTF/DejaVuSansMono.ttf"}},
        {"font_family",  {ValueType::STRING, "DejaVu Sans Mono"}}, // Used if use_font_path is FALSE
        {"font_size",    {ValueType::INT,    "14"}},        // Font size
        {"tab_spaces",   {ValueType::INT,    "4"}},         // number of spaces to use for tabs
        {"line_numbers", {ValueType::BOOL,   "TRUE"}},     // Show line numbers
        {"use_font_path",{ValueType::BOOL,   "TRUE"}},      // Use fully qualified font path
        {"block_cursor", {ValueType::BOOL,   "TRUE"}},      // Show block or line cursor
        {"hl_cur_line",  {ValueType::BOOL,   "TRUE"}},      // Highlight the line the caret is on
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
            status = (val == "true" || val == "false" || val == "TRUE" || val == "FALSE");
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
