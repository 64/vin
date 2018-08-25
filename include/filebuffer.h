#ifndef FILEBUFFER_H
#define FILEBUFFER_H

#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <stack>

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

enum class Action
{
    INSERT,
    REMOVE
};

struct Cursor
{
    Cursor(int _x, int _y)
        : advance{0}, x{0},
          y{0} , offset{0},
          hard_x{0} {}

    int         advance;
    int         x, y;
    int         hard_x;
    std::size_t offset;
};

struct BufferState
{
    Cursor      cur;
    int         num_lines;
    std::size_t num_chars;
};

struct Operation
{
    Action      action;
    std::size_t length;
    Span*       span;
    BufferState state;
    Span*       first;
    Span*       last;
};

class FileBuffer
{
public:
    FileBuffer(const std::string& file_name,
               const Vec2i& orig, FontFace* font);

    int line_count();
    void move_pos(Move dir);
    void del();
    void backspace();
    void tab();
    void new_line();
    void ins_char(unsigned int ch);
    void jump_to_caret();
    void save_to_file();
    int& offset();
    int ch(int offset = 0);
    int ch_width(int offset = 0);
    Vec2i draw_pos();
    Span* buffer_data();

private:
    void set_state(const BufferState& state);
    BufferState get_state();
    void forward();
    void backward();
    void downward();
    void upward();
    void calc_short_line();
    void check_for_offset();
    std::pair<int, int> line_width();

private:
    std::string           name;
    Sequence              data;
    int                   num_lines;
    Cursor                cur;
    Vec2i                 orig;
    FontFace*             font;
    int                   scroll_offset;
    std::size_t           num_chars;
    bool                  append;
    std::stack<Operation> undo_stack;

};

#endif
