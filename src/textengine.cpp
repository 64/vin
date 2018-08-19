#include "textengine.h"
#include "renderer.h"
#include "util.h"

TextEngine::TextEngine(Renderer& _renderer, FontFace& _font, int offset, int _fg_color, int _cl_color, int _cr_color, bool _hl_cur_line)
    : renderer(_renderer), cur(5, offset), buffer(""), fg_color(rgb_to_vec(_fg_color)), cl_color(rgb_to_vec(_cl_color)),
      cr_color(rgb_to_vec(_cr_color)), origin(5, offset), font(_font), hl_cur_line(_hl_cur_line)
{

}

void TextEngine::render()
{
    Vec2i pos = origin;

    if (hl_cur_line)
        renderer.draw_character(1, 0, cur.pos().y, cl_color); // Current Line

    renderer.draw_text(buffer, pos.x, pos.y, fg_color);   // Buffer text
    renderer.draw_character(0, pos.x, pos.y, cr_color);   // Cursor

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
                    cur.pos().y -= font.font_height();
                buffer.pop_back();
            }
        } break;

        case '\n':
        {
            cur.pos().y += font.font_height();
        }

        default:
        {
            buffer += ch;
        } break;
    }

}
