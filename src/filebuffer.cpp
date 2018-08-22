#include "filebuffer.h"

FileBuffer::FileBuffer(const std::string& file_name, const Vec2i& _orig, FontFace* _font)
    : name(file_name), num_lines(0), cur(0, 0/*, data.begin()*/), orig(_orig), font(_font),
      scroll_offset(0), num_chars(0), data{file_name}
{
    for (const auto& span : data.pieces())
        for (std::size_t i = 0; i < span.length; ++i)
        {
            if (span.start[i] == '\n')
                ++num_lines;
            ++num_chars;
        }

    if (!num_lines)
        num_lines = 1;
}

void FileBuffer::save_to_file()
{
    // TODO
}

const std::list<Span>& FileBuffer::buffer_data()
{
    return data.pieces();
}

void FileBuffer::ins_char(unsigned int ch)
{
    // TODO
}

void FileBuffer::forward()
{
    if (cur.offset < num_chars - 1)
    {
        if (ch() == '\n' && cur.y < num_lines - 1)
        {
            ++cur.y;
            cur.x = 0;
            cur.hard_x = 0;
            cur.advance = 0;
        }
        else
        {
            cur.advance += ch_width();
            ++cur.x;
            ++cur.hard_x;
        }

        ++cur.offset;
    }
}

void FileBuffer::backward()
{
    if (cur.offset > 0)
    {
        --cur.offset;

        if (ch() == '\n')
        {
            --cur.y;
            auto size   = line_width();
            cur.advance = size.second;
            cur.x       = size.first - 1;
            cur.hard_x  = size.first - 1;
        }
        else
        {
            cur.advance -= ch_width();
            --cur.x;
            --cur.hard_x;
        }
    }
}

void FileBuffer::downward()
{
    // TODO
}

void FileBuffer::upward()
{
    if (cur.y)
    {
        --cur.y;
        --cur.offset;
        while (ch() != '\n')
            --cur.offset;

        auto size   = line_width();
        cur.x       = std::min(cur.hard_x, --size.first);
        cur.advance = size.second;

        for (int i = size.first; i > cur.x; --i, --cur.offset)
            cur.advance -= ch_width();
    }
}

int FileBuffer::line_count()
{
    return num_lines;
}

Vec2i FileBuffer::draw_pos()
{
    return {orig.x + cur.advance, (cur.y + 1) * (font->font_height())};
}

void FileBuffer::del()
{
    // TODO
}

void FileBuffer::tab()
{
    for (int i = 0; i < font->num_spaces(); ++i)
        ins_char(' ');
}

void FileBuffer::backspace()
{
    // TODO
}

void FileBuffer::new_line()
{
    // TODO
}

std::pair<int, int> FileBuffer::line_width()
{
    std::size_t total = 0;
    auto it = data.get_span(cur.offset, total);
    int advance = 0;
    int chars   = 0;
    std::size_t length = it->length - (total + it->length - cur.offset);
    for (int i = 0; i != '\n'; i = it->start[length])
    {
        advance += i ? font->get_glyph(it->start[length]).advancex >> 6 : 0;
        ++chars;

        if (length-- == 0)
        {
            --it;
            length = it->length;
        }

        if (it->start + length == nullptr)
            break;
    }

    return {chars, advance};
}

void FileBuffer::calc_short_line()
{
    // TODO
}

void FileBuffer::jump_to_caret()
{
    // TODO
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

int FileBuffer::ch(int offset)
{
    data.get_ch(cur.offset + offset);
}

int& FileBuffer::offset()
{
    return scroll_offset;
}

