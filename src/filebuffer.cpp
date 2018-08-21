#include <string>
#include <fstream>
#include <algorithm>

#include "filebuffer.h"

#define X    cur.pos().x    // Column
#define Y    cur.pos().y    // Row
#define LINE cur.line()     // Current line

FileBuffer::FileBuffer(const std::string& file_name, const Vec2i& _orig, FontFace* _font)
    : name(file_name), num_lines(0), cur(0, 0, lines.begin()), orig(_orig), font(_font),
      scroll_offset(0), saved_x(0)
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
    LINE = lines.begin();
}

void FileBuffer::ins_char(unsigned int ch)
{
    LINE->insert(LINE->begin() + X, &ch, &ch + 1);
    move_pos(Move::RIGHT);
}

const std::list<GapBuffer<char>>& FileBuffer::get_lines()
{
    return lines;
}

Vec2i FileBuffer::draw_pos()
{
    return {line_width(X) + orig.x, static_cast<int>((font->font_height() * (Y + 1)) - scroll_offset)};
}

void FileBuffer::del()
{
    auto it = std::next(LINE);
    if (LINE->at(X) == '\n' && it != lines.end())
    {
        LINE->erase(LINE->begin() + X);
        LINE->insert(LINE->begin() + X, it->begin(), it->end());
        lines.erase(it);
    }
    else if (LINE == std::prev(lines.end()) && X == LINE->size())
    {
        return;
    }
    else if (!LINE->empty())
    {
        LINE->erase(LINE->begin() + X);
    }
    check_for_offset();
}

void FileBuffer::tab()
{
    for (int i = 0; i < font->num_spaces(); ++i)
        ins_char(' ');
}

void FileBuffer::backspace()
{
    if (X > 0)
    {
        move_pos(Move::LEFT);
        LINE->erase(LINE->begin() + X);
    }
    else if (LINE != lines.begin())
    {
        --LINE;
        --Y;
        X = LINE->size() - 1;
        auto it = std::next(LINE);
        LINE->pop_back();
        LINE->insert(LINE->end(), it->begin(), it->end());
        lines.erase(it);
        check_for_offset();
    }
}

void FileBuffer::new_line()
{
    LINE->insert(LINE->begin() + ++X, '\n');
    lines.emplace(std::next(LINE), 0, 20);
    auto it = std::next(LINE);
    it->insert(it->begin(), LINE->begin() + X, LINE->end());
    LINE->erase(LINE->begin() + X, LINE->end());
    ++LINE;
    ++Y;
    X = 0;
    check_for_offset();
}

int FileBuffer::line_width(int delim)
{
    int total = 0;

    if (delim != -1)
        for (int i = 0; i < delim; ++i)
            total += font->get_glyph(LINE->at(i)).advancex >> 6;
    else
        std::for_each(LINE->begin(), LINE->end(), [&total, this](char ch)
        {if (ch != '\n') total += font->get_glyph(ch).advancex >> 6;});

    return total;
}

void FileBuffer::calc_short_line()
{
    if (X > LINE->size() - 1)
        X = LINE->size() - 1;
}

void FileBuffer::jump_to_caret()
{
    if (draw_pos().y < 0)
    {
        scroll_offset += draw_pos().y - font->font_height() * 1;
        scroll_offset = scroll_offset < 0 ? 0 : scroll_offset;
    }
    else if (draw_pos().y > SCR_HEIGHT)
    {
        scroll_offset += draw_pos().y - font->font_height() * (SCR_HEIGHT / font->font_height());
    }
}

void FileBuffer::move_pos(Move dir)
{
    switch (dir)
    {
        case Move::UP:
            if ((font->font_height() * (Y + 1)) - font->font_height() < scroll_offset)
                scroll_offset -= font->font_height();
            if (LINE != lines.begin())
            {
                --LINE;
                --Y;
                calc_short_line();
                check_for_offset();
            }
            jump_to_caret();
            break;

        case Move::DOWN:
            if (Y < lines.size() - 1)
            {
                ++LINE;
                ++Y;
                calc_short_line();
                check_for_offset();
            }
            jump_to_caret();
            break;

        case Move::LEFT:
            if (X > 0)
            {
                --X;
            }
            jump_to_caret();
            break;

        case Move::RIGHT:
            if (X < LINE->size())
            {
                if (LINE->at(X) != '\n')
                    ++X;
            }
            jump_to_caret();
            break;
    }
}

void FileBuffer::check_for_offset()
{
    int pos = (font->font_height() * (Y + 1));
    if (pos > SCR_HEIGHT + scroll_offset)
        scroll_offset += font->font_height();
    else if (pos - font->font_height() < scroll_offset)
        scroll_offset -= font->font_height();
}

int FileBuffer::ch_width()
{
    if (!LINE->empty())
    {
        Glyph ch = font->get_glyph(LINE->at(X));
        if (ch.advancex)
            return ch.advancex >> 6;
        else
            return font->font_width();
    }
}

int FileBuffer::ch()
{
    return LINE->at(X);
}

int& FileBuffer::offset()
{
    return scroll_offset;
}

