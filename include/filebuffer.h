#ifndef FILEBUFFER_H
#define FILEBUFFER_H

#include <list>
#include <string>

#include "gap_buffer.h"
#include "fontface.h"
#include "util.h"

enum class Move
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Cursor
{
    using line_t = std::list<GapBuffer<char>>::iterator;
public:
    Cursor(int _x, int _y, line_t _line) : coords{_x, _y}, buffer{_line} {}
    Vec2i& pos()   { return coords; }
    line_t& line() { return buffer; }

private:
    Vec2i coords;
    line_t buffer;
};

class FileBuffer
{
public:
    FileBuffer(const std::string& file_name, const Vec2i& orig, FontFace* font);
    const std::list<GapBuffer<char>>& get_lines();
    void calc_short_line();
    Vec2i draw_pos();
    void move_pos(Move dir);
    void del();
    void backspace();
    void new_line();
    int line_width();
    void ins_char(unsigned int ch);
    int line_count();


private:
    Vec2i draw_coords;
    std::string name;
    std::list<GapBuffer<char>> lines;
    Cursor cur;
    int num_lines;
    Vec2i orig;
    FontFace* font;

};

#endif
