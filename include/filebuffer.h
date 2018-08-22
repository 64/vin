#ifndef FILEBUFFER_H
#define FILEBUFFER_H

#include <list>
#include <string>

#include "gap_buffer.h"
#include "fontface.h"
#include "util.h"
#include "piecetable.h"

enum class Move
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Cursor
{
    using line_t = GapBuffer<char>::iterator;
public:
    Cursor(int _x, int _y/*, line_t _line*/)
        : coords{_x, _y}, /*buffer{_line},*/
          advance{0}, /*line_y{0}, col{0}*/
          x{0}, y{0} , offset{0} {}

public:
    Vec2i coords;
//    line_t buffer;
    int advance;
    int x, y, offset;
//    int line_y;
//    int col;
};

class FileBuffer
{
public:
    FileBuffer(const std::string& file_name, const Vec2i& orig, FontFace* font);
    int line_count();
    Vec2i draw_pos();
    void move_pos(Move dir);
    void del();
    void backspace();
    void tab();
    void new_line();
    const std::list<Span>& buffer_data();
    std::pair<int, int> line_width(int delim = -1);
    int& offset();
    void ins_char(unsigned int ch);
    int ch_width(int offset = 0);
    void jump_to_caret();
    void save_to_file();
    int ch();
    void forward();
    void backward();
    void downward();
    void upward();

private:
    void calc_short_line();
    void check_for_offset();

private:
    std::string name;
    Sequence data;
//    GapBuffer<char> data;
    int num_lines;
    Cursor cur;
    Vec2i orig;
    FontFace* font;
    int scroll_offset;
    int saved_x;

};

#endif
