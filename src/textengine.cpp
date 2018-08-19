#include <string>
#include <iostream>

#include "textengine.h"
#include "renderer.h"
#include "util.h"

TextEngine::TextEngine(Renderer& _renderer, FontFace& _font, int offset, int _fg_color, int _cl_color, int _cr_color,
                       int _ln_color, int _gt_color, bool _hl_cur_line, bool _line_numbers)
    : renderer(_renderer), font(_font), cur(5, offset), buffer(""), origin(5, offset), fg_color(rgb_to_vec(_fg_color)),
      cl_color(rgb_to_vec(_cl_color)), cr_color(rgb_to_vec(_cr_color)), ln_color(rgb_to_vec(_ln_color)),
      gt_color(rgb_to_vec(_gt_color)),  hl_cur_line(_hl_cur_line), lines(0), line_numbers(_line_numbers)
{

    cur.pos() = line_numbers ? Vec2i{ origin.x + GUTTER_WIDTH, origin.y } : origin;
}

void TextEngine::render()
{
    Vec2i pos = line_numbers ? Vec2i{ origin.x + GUTTER_WIDTH, origin.y } : origin;

    if (hl_cur_line)
        renderer.draw_character(1, 0, cur.pos().y, cl_color); // Current Line

    pos = renderer.draw_text(buffer, pos.x, pos.y, fg_color, line_numbers);   // Buffer text
    renderer.draw_character(0, pos.x, pos.y, cr_color);   // Cursor

    if (line_numbers)
    {
        renderer.draw_character(2, 0, 0, gt_color);
        for (int i = 0; i <= lines; ++i)
        {
            // GL with this shit
            std::string num = std::to_string(i + 1);
            int calc_x = (font.font_width() * 3) - ((num.size() - 1) * font.font_width());
            renderer.draw_text(num, calc_x, (i + 1) * font.font_height(), ln_color, false);
        }
    }
}

void TextEngine::process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void TextEngine::append(int ch)
{
    switch (ch)
    {
        case '\b':
        {
            if (!buffer.empty())
            {
                if (buffer.back() == '\n')
                {
                    cur.pos().y -= font.font_height();
                    --lines;
                }
                buffer.pop_back();
            }
        } break;

        case '\n':
        {
            buffer += ch;
            cur.pos().y += font.font_height();
            ++lines;
        } break;

        default:
        {
            buffer += ch;
        } break;
    }

}
