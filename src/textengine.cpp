#include <string>
#include <fstream>

#include "textengine.h"
#include "renderer.h"
#include "util.h"

TextEngine::TextEngine(Renderer& _renderer, FontFace& _font, int offset, int _fg_color, int _cl_color, int _cr_color,
                       int _ln_color, int _gt_color, bool _hl_cur_line, bool _line_numbers)
    : renderer(_renderer), font(_font), buffer(""), origin(5, offset), fg_color(rgb_to_vec(_fg_color)),
      cl_color(rgb_to_vec(_cl_color)), cr_color(rgb_to_vec(_cr_color)), ln_color(rgb_to_vec(_ln_color)),
      gt_color(rgb_to_vec(_gt_color)), hl_cur_line(_hl_cur_line), line_numbers(_line_numbers), num_lines(0)
{
//    cur.pos() = line_numbers ? Vec2i{ origin.x + GUTTER_WIDTH, origin.y } : origin;
    buffers.emplace(buffers.cend(), "main.cpp", line_numbers ? Vec2i{ origin.x + GUTTER_WIDTH, origin.y } : origin, &font);
    active_buffer = buffers.begin();
    GapBuffer<char> buf {0, 8};
    buf.push_back('z');
}

void TextEngine::render()
{
    Vec2i pos = line_numbers ? Vec2i{ origin.x + GUTTER_WIDTH, origin.y } : origin;

//    if (hl_cur_line)
//        renderer.draw_character(1, 0, cur.pos().y, cl_color); // Current Line

    for (const auto& line : buffers[0].get_lines())
    {
        pos = renderer.draw_text(line.string(false), pos, fg_color, line_numbers); // Buffer text
    }

    renderer.draw_character(0, active_buffer->draw_pos(), cr_color);   // Cursor

    if (line_numbers)
    {
        renderer.draw_character(2, {0, 0}, gt_color); // Gutter
        for (int i = 0; i < active_buffer->line_count(); ++i)
        {
            // GL with this shit
            std::string num = std::to_string(i + 1);
            int calc_x = (font.font_width() * 3) - ((num.size() - 1) * font.font_width());
            renderer.draw_text(num, {calc_x, (i + 1) * font.font_height()}, ln_color, false);
        }
    }
}

void TextEngine::process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void TextEngine::append(unsigned int ch)
{
    switch (ch)
    {
        case GLFW_KEY_UP:
            active_buffer->move_pos(Move::UP);
            break;

        case GLFW_KEY_DOWN:
            active_buffer->move_pos(Move::DOWN);
            break;

        case GLFW_KEY_LEFT:
            active_buffer->move_pos(Move::LEFT);
            break;

        case GLFW_KEY_RIGHT:
            active_buffer->move_pos(Move::RIGHT);
            break;

        case GLFW_KEY_DELETE:
            active_buffer->del();
            break;

        case GLFW_KEY_ENTER:
            active_buffer->new_line();
            break;

        case '\b':
            active_buffer->backspace();
            break;

        default:
            active_buffer->ins_char(ch);
    }
}
