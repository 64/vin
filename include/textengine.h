#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include <GLFW/glfw3.h>

#include "util.h"
#include "renderer.h"

class Cursor
{
public:
    Cursor(int _x, int _y) : coords{_x, _y} {}
    Vec2i& pos() { return coords; }

private:
    Vec2i coords;
};

class TextEngine
{
public:
    TextEngine(Renderer& _renderer, FontFace& _font, int offset, int _fg_color,
               int _cl_color, int _cr_color, int _ln_color, int _gt_color, bool _hl_cur_line, bool _line_numbers);
    void render();
    void process_input(GLFWwindow *window);
    void append(int ch);
    void character_callback(GLFWwindow* window, unsigned int codepoint);

private:
    Renderer& renderer;
    FontFace& font;
    Cursor cur;
    std::string buffer; // This will be replaced by the file buffers and is only for the typing test
    Vec2i origin;
    Vec3f fg_color;
    Vec3f cl_color;
    Vec3f cr_color;
    Vec3f ln_color;
    Vec3f gt_color;
    bool hl_cur_line;
    bool line_numbers;
    int lines;
};

#endif
