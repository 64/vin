#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include <GLFW/glfw3.h>

#include <vector>

#include "util.h"
#include "renderer.h"
#include "gap_buffer.h"
#include "filebuffer.h"

class TextEngine
{
public:
    TextEngine(Renderer& _renderer, FontFace& _font, int offset, int _fg_color,
               int _cl_color, int _cr_color, int _ln_color, int _gt_color, bool _hl_cur_line, bool _line_numbers);
    void render();
    void process_input(GLFWwindow *window);
    void append(unsigned int ch);
    void character_callback(GLFWwindow* window, unsigned int codepoint);

private:
    Renderer& renderer;
    FontFace& font;
    std::string buffer; // This will be replaced by the file buffers and is only for the typing test
    Vec2i origin;
    Vec3f fg_color;
    Vec3f cl_color;
    Vec3f cr_color;
    Vec3f ln_color;
    Vec3f gt_color;
    bool hl_cur_line;
    bool line_numbers;
    int num_lines;
    std::vector<FileBuffer> buffers;
    std::vector<FileBuffer>::iterator active_buffer;
};

#endif
