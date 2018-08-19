#include "textengine.h"
#include "renderer.h"
#include <iostream>

TextEngine::TextEngine(int offset, Renderer& _renderer)
    : renderer(_renderer), cur(5, offset), buffer("")
{

}

void TextEngine::render()
{
    Vec2i pos = cur.pos();
    renderer.draw_text(buffer, pos.x, pos.y);
    renderer.draw_character(0, pos.x, pos.y);
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
                buffer.pop_back();
        } break;

        default:
        {
            buffer += ch;
        } break;
    }

}
