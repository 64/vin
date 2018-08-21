#include <string>
#include <fstream>
#include <cctype>

#include "textengine.h"
#include "renderer.h"
#include "util.h"

TextEngine::TextEngine(Renderer& _renderer, FontFace& _font, int offset, int _bg_color, int _fg_color, int _cl_color, int _cr_color,
                       int _ln_color, int _gt_color, bool _hl_cur_line, bool _line_numbers, bool _block_caret)
    : renderer(_renderer), font(_font), buffer(""), origin(5, offset), bg_color(rgb_to_vec(_bg_color)), fg_color(rgb_to_vec(_fg_color)),
      cl_color(rgb_to_vec(_cl_color)), cr_color(rgb_to_vec(_cr_color)), ln_color(rgb_to_vec(_ln_color)),
      gt_color(rgb_to_vec(_gt_color)), hl_cur_line(_hl_cur_line), line_numbers(_line_numbers), block_caret(_block_caret)
{
    buffers.emplace(buffers.cend(), "main.cpp", line_numbers ? Vec2i{ origin.x + GUTTER_WIDTH, origin.y } : origin, &font);
    active_buffer = buffers.begin();
    GapBuffer<char> buf {0, 8};
    buf.push_back('z');
}

void TextEngine::render()
{
    int offset = active_buffer->offset();
    Vec2i pos = line_numbers ? Vec2i{ origin.x + GUTTER_WIDTH, origin.y } : origin;
    pos.y -= offset;

    // Highlight current line
    if (hl_cur_line)
        renderer.draw_rectangle({0, active_buffer->draw_pos().y + font.font_cleft(), SCR_WIDTH, font.font_height() + font.font_cleft()}, cl_color);

    // Draw onscreen text
    for (const auto& line : active_buffer->get_lines())
    {
        pos = renderer.draw_text(line.string(false), pos, fg_color, line_numbers); // Buffer text
    }

    // Draw caret
    Vec2i&& cur = active_buffer->draw_pos();
    int caret_width = block_caret ? active_buffer->ch_width() : 2;
    renderer.draw_rectangle({cur.x, cur.y + font.font_cleft(), caret_width, font.font_height() + font.font_cleft()}, cr_color);

    // Draw current character over the block caret for visibility
    int c = active_buffer->ch();
    if (block_caret && !std::isspace(c) && c)
        renderer.draw_character(c, {cur.x, cur.y - font.font_cleft()}, bg_color);

    if (line_numbers)
    {
        // Draw gutter
        renderer.draw_rectangle({0, 0, GUTTER_WIDTH, -SCR_HEIGHT}, gt_color);
        for (unsigned int i = 0; i < active_buffer->get_lines().size(); ++i)
        {
            std::string num = std::to_string(i + 1);
            int calc_x = (font.font_width() * 3) - ((num.size() - 1) * font.font_width());
            renderer.draw_text(num, {calc_x, static_cast<int>((i + 1) * font.font_height() - offset) - font.font_cleft()}, ln_color, false);
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

        case '\t':
            active_buffer->tab();
            break;

        case 321:
            if (active_buffer->offset() > 0)
                active_buffer->offset() -= font.font_height();
                break;

        case 320:
            if (active_buffer->offset() < ((active_buffer->get_lines().size() - 1) * font.font_height()))
                active_buffer->offset() += font.font_height();
                break;


        default:
            active_buffer->ins_char(ch);
    }
}
