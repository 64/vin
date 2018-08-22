#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "filebuffer.h"

#define Y    cur.y    // Row
#define LINE cur.line()     // Current line

FileBuffer::FileBuffer(const std::string& file_name, const Vec2i& _orig, FontFace* _font)
    : name(file_name), num_lines(0), cur(0, 0/*, data.begin()*/), orig(_orig), font(_font),
      scroll_offset(0), saved_x(0), data{file_name}
{

}

void FileBuffer::save_to_file()
{
//    std::ofstream file { name };

//    for (const auto ch : data)
//        file << ch;

//    file.close();
}

const std::list<Span>& FileBuffer::buffer_data()
{
    return data.pieces();
}

void FileBuffer::ins_char(unsigned int ch)
{
//    LINE->insert(LINE->begin() + X, &ch, &ch + 1);
//    move_pos(Move::RIGHT);
}

void FileBuffer::forward()
{
//    if (cur.buffer != data.end())
//    {
//        cur.advance += ch_width();
//        ++cur.buffer;
//        ++cur.col;
//    }

//    if (*(cur.buffer - 1) == '\n')
//    {
//        ++cur.line_y;
//        cur.col = 0;
//        cur.advance = 0;
//    }
}

void FileBuffer::backward()
{
//    if (cur.buffer != data.begin())
//    {
//        cur.advance -= ch_width(-1);
//        --cur.buffer;
//        --cur.col;
//    }

//    if (*cur.buffer == '\n')
//    {
//        --cur.line_y;
//        cur.advance = line_width().second;
//        cur.col = line_width().first;
//    }
}

void FileBuffer::downward()
{
//    if (cur.buffer == data.end() - 1)
//        return;

//    while (*cur.buffer++ != '\n' && cur.buffer != data.end())
//        ;

//    int new_col = 0;
//    for (int i = 0;
//         cur.buffer != data.end() - 1 && *cur.buffer != '\n' && i < cur.col;
//         ++i, ++new_col, ++cur.buffer);

//    cur.advance = line_width().second;
//    cur.col = new_col;
//    ++cur.line_y;
}

void FileBuffer::upward()
{
//    if (cur.buffer == data.begin())
//        return;

//    while (*cur.buffer-- != '\n' && cur.buffer != data.begin())
//        ;

//    auto width = line_width();
////    for (int i = 0; *cur.buffer-- != '\n' && width.first > cur.col; --width.first, --cur.buffer);

//    cur.advance = width.second;
//    cur.col = width.first;
//    --cur.line_y;
}

int FileBuffer::line_count()
{
    return num_lines + 1;
}

Vec2i FileBuffer::draw_pos()
{
//    return {line_width(X) + orig.x, static_cast<int>((font->font_height() * (Y + 1)) - scroll_offset)};
//    std::cout << font->font_height() << std::endl;
    return {orig.x + cur.advance, (cur.y + 1) * (font->font_height())};
}

void FileBuffer::del()
{
//    auto it = std::next(LINE);
//    if (LINE->at(X) == '\n' && it != lines.end())
//    {
//        LINE->erase(LINE->begin() + X);
//        LINE->insert(LINE->begin() + X, it->begin(), it->end());
//        lines.erase(it);
//    }
//    else if (LINE == std::prev(lines.end()) && X == LINE->size())
//    {
//        return;
//    }
//    else if (!LINE->empty())
//    {
//        LINE->erase(LINE->begin() + X);
//    }
//    check_for_offset();
}

void FileBuffer::tab()
{
    for (int i = 0; i < font->num_spaces(); ++i)
        ins_char(' ');
}

void FileBuffer::backspace()
{
//    if (X > 0)
//    {
//        move_pos(Move::LEFT);
//        LINE->erase(LINE->begin() + X);
//    }
//    else if (LINE != lines.begin())
//    {
//        --LINE;
//        --Y;
//        X = LINE->size() - 1;
//        auto it = std::next(LINE);
//        LINE->pop_back();
//        LINE->insert(LINE->end(), it->begin(), it->end());
//        lines.erase(it);
//        check_for_offset();
//    }
}

void FileBuffer::new_line()
{
//    LINE->insert(LINE->begin() + ++X, '\n');
//    lines.emplace(std::next(LINE), 0, 20);
//    auto it = std::next(LINE);
//    it->insert(it->begin(), LINE->begin() + X, LINE->end());
//    LINE->erase(LINE->begin() + X, LINE->end());
//    ++LINE;
//    ++Y;
//    X = 0;
//    check_for_offset();
}

std::pair<int, int> FileBuffer::line_width(int delim)
{
//    int advance = 0;
//    int chars   = 0;
//    auto it = cur.buffer;

//    while (*(--cur.buffer) != '\n' && cur.buffer != data.begin() - 1)
//    {
//        advance += ch_width();
//        ++chars;
//    }

//    cur.buffer = it;

//    return {chars, advance};
}

void FileBuffer::calc_short_line()
{
//    if (X > LINE->size() - 1)
//        X = LINE->size() - 1;
}

void FileBuffer::jump_to_caret()
{
//    if (draw_pos().y < 0)
//    {
//        scroll_offset += draw_pos().y - font->font_height() * 1;
//        scroll_offset = scroll_offset < 0 ? 0 : scroll_offset;
//    }
//    else if (draw_pos().y > SCR_HEIGHT)
//    {
//        scroll_offset += draw_pos().y - font->font_height() * (SCR_HEIGHT / font->font_height());
//    }
}

void FileBuffer::move_pos(Move dir)
{
    switch (dir)
    {
        case Move::UP:
            upward();
            break;

        case Move::DOWN:
            downward();
            break;

        case Move::LEFT:
            backward();
            break;

        case Move::RIGHT:
            forward();
            break;
    }
}

void FileBuffer::check_for_offset()
{
//    int pos = (font->font_height() * (Y + 1));
//    if (pos > SCR_HEIGHT + scroll_offset)
//        scroll_offset += font->font_height();
//    else if (pos - font->font_height() < scroll_offset)
//        scroll_offset -= font->font_height();
}

int FileBuffer::ch_width(int offset)
{
        Glyph ch = font->get_glyph(data.get_ch(cur.offset));
        if (ch.advancex)
            return ch.advancex >> 6;
        else
            return font->font_width();
}

int FileBuffer::ch()
{
    data.get_ch(cur.offset);
}

int& FileBuffer::offset()
{
    return scroll_offset;
}

