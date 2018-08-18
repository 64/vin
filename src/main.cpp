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

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main()
{
#ifdef __DEBUG__
    std::cout << "Vin Editor Debug Build" << std::endl;
    std::cout << "Compiled on " << __DATE__ << " at " << __TIME__ << std::endl;
#endif

//    message_box("Crally", "This is a message box with X11");

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
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    FT_Library library;
    if (FT_Init_FreeType(&library))
    {
        std::cerr << "Failed to initialize Freetype" << std::endl;
        return EXIT_FAILURE;
    }

    Renderer renderer;
    Font font(library, config.option("font_path"), 100);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        process_input(window);

        renderer.draw_character(window, 'G', 500, 500, font);
        renderer.draw_character(window, 'A', 600, 500, font);
        renderer.draw_character(window, 'Y', 670, 500, font);

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    FT_Done_FreeType(library);
    glfwTerminate();
    return EXIT_SUCCESS;
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
