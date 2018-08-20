#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "filebuffer.h"

FileBuffer::FileBuffer(const std::string& file_name, const Vec2i& _orig, FontFace* _font)
    : name(file_name), num_lines(0), cur(0, 0, lines.begin()), orig(_orig), draw_coords(_orig), font(_font)
{
    std::ifstream file { file_name };
    if (file)
    {
        std::string line;
        while (std::getline(file, line))
        {
            line.insert(line.cend(), '\n');
            lines.emplace_back(0, 20);
            lines.back().insert(lines.back().begin(), line.begin(), line.end());
            ++num_lines;
        }

        // Remove trailing newline character from last line
        std::prev(lines.end())->pop_back();
        file.close();
    }
    else
    {
        lines.emplace_back(0, 20);
    }

    // Set current line to first line
    cur.line() = lines.begin();
}

void FileBuffer::ins_char(unsigned int ch)
{
   cur.line()->insert(cur.line()->begin() + cur.pos().x , &ch, &ch + 1);
   move_pos(Move::RIGHT);
}

const std::list<GapBuffer<char>>& FileBuffer::get_lines()
{
    return lines;
}

Vec2i FileBuffer::draw_pos()
{
    return draw_coords;
}

void FileBuffer::del()
{
    auto it = std::next(cur.line());
    if (cur.line()->at(cur.pos().x) == '\n' && it != lines.end())
    {
        cur.line()->erase(cur.line()->begin() + cur.pos().x);
        cur.line()->insert(cur.line()->begin() + cur.pos().x, it->begin(), it->end());
        lines.erase(it);
    }
    else if (cur.line() == std::prev(lines.end()) && cur.pos().x == cur.line()->size())
    {
        return;
    }
    else if (!cur.line()->empty())
    {
        cur.line()->erase(cur.line()->begin() + cur.pos().x);
    }
}

void FileBuffer::backspace()
{
    if (cur.pos().x > 0)
    {
        move_pos(Move::LEFT);
        cur.line()->erase(cur.line()->begin() + cur.pos().x);
    }
    else if (cur.line() != lines.begin())
    {
        --cur.line();
        --cur.pos().y;
        cur.pos().x = cur.line()->size() - 1;
        draw_coords.x = line_width() + orig.x;
        draw_coords.y -= font->font_height();
        auto it = std::next(cur.line());
        cur.line()->pop_back();
        cur.line()->insert(cur.line()->end(), it->begin(), it->end());
        lines.erase(it);
    }
}

void FileBuffer::new_line()
{
    cur.line()->insert(cur.line()->begin() + cur.pos().x + 1, '\n');
    ++cur.pos().x;
    lines.emplace(std::next(cur.line()), 0, 20);
    auto it = std::next(cur.line());
    it->insert(it->begin(), cur.line()->begin() + cur.pos().x, cur.line()->end());
    cur.line()->erase(cur.line()->begin() + cur.pos().x, cur.line()->end());
    ++cur.line();
    cur.pos().x = 0;
    ++cur.pos().y;
    draw_coords.y += font->font_height();
    draw_coords.x = orig.x;
}

int FileBuffer::line_width()
{
    int total = 0;
    std::for_each(cur.line()->begin(), cur.line()->end(), [&total, this](char ch)
    {
        if (ch != '\n')
            total += font->get_glyph(ch).advancex >> 6;
    });
    return total;
}

void FileBuffer::calc_short_line()
{
    if (cur.pos().x > cur.line()->size() - 1)
    {
        cur.pos().x = cur.line()->size() - 1;
        draw_coords.x = line_width() + orig.x;
    }
}

void FileBuffer::move_pos(Move dir)
{
    switch (dir)
    {
        case Move::UP:
            if (cur.line() != lines.begin())
            {
                --cur.line();
                --cur.pos().y;
                draw_coords.y -= font->font_height();
                calc_short_line();
            } break;

        case Move::DOWN:
            if (cur.pos().y < lines.size() - 1)
            {
                ++cur.line();
                ++cur.pos().y;
                draw_coords.y += font->font_height();
                calc_short_line();
            } break;

        case Move::LEFT:
            if (cur.pos().x > 0)
            {
                --cur.pos().x;
                draw_coords.x -= font->get_glyph(cur.line()->at(cur.pos().x)).advancex >> 6;
            } break;

        case Move::RIGHT:
            if (cur.pos().x < cur.line()->size())
            {
                if (cur.line()->at(cur.pos().x) != '\n')
                {
                    draw_coords.x += font->get_glyph(cur.line()->at(cur.pos().x)).advancex >> 6;
                    ++cur.pos().x;
                }
            } break;
    }
}

int FileBuffer::line_count()
{
    return lines.size();
}

