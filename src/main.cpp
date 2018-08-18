#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <cstdlib>
#include <string>

#include "configreader.h"
#include "renderer.h"
#include "font.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
void message_box(const std::string& title, const std::string& msg);

[[noreturn]] void error(const std::string& msg);

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main(int, char**)
{
#ifdef __DEBUG__
    std::cout << "Vin Editor Debug Build" << std::endl;
    std::cout << "Compiled on " << __DATE__ << " at " << __TIME__ << std::endl;
#endif

    ConfigReader config;
    config.load();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Vin Editor", nullptr, nullptr);
    if (!window)
        error("Failed to create GLFW window");

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        error("Failed to initialize GLAD");

    FT_Library library;
    if (FT_Init_FreeType(&library))
        error("Failed to initialize Freetype");

    Renderer renderer;
    Font font(library, config.option("font_path"), 14);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        process_input(window);

        renderer.draw_text(window, "The quick brown fox jumps over the lazy log", 300, 300, font);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    FT_Done_FreeType(library);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void error(const std::string& msg)
{
    std::cerr << msg << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
}

void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}
