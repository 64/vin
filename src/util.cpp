#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "util.h"

Vec3f rgb_to_vec(int color)
{
    return Vec3f {
        static_cast<float>(((color >> 16) & 0xFF)) / 255.0f,
        static_cast<float>((color >> 8) & 0xFF)    / 255.0f,
        static_cast<float>((color & 0x00FF))       / 255.0f,
    };
}

void error(const std::string& msg)
{
    std::cerr << msg << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
}
