#ifndef TEXTENGINE_H
#define TEXTENGINE_H

#include <GLFW/glfw3.h>

#include "util.h"
#include "renderer.h"

class Cursor
{
public:
    Cursor(int _x, int _y) : coords{_x, _y} {}
    Vec2i pos() { return coords; }

private:
    Vec2i coords;
};

class TextEngine
{
public:
    TextEngine(int offset, Renderer& renderer);
    void render();
    void process_input(GLFWwindow *window);
    void append(int ch);
    void character_callback(GLFWwindow* window, unsigned int codepoint);

private:
    Renderer& renderer;
    Cursor cur;
    std::string buffer;

};

#endif
