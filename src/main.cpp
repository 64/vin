#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <queue>
#include <cctype>

#include "configreader.h"
#include "renderer.h"
#include "fontface.h"
#include "textengine.h"

void process_input(GLFWwindow *window);
void message_box(const std::string& title, const std::string& msg);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

static std::queue<unsigned int> buffer;

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
        message_box("Error", "Failed to create GLFW window", true);


    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        message_box("Error", "Failed to initialize GLAD", true);

    FT_Library library;
    if (FT_Init_FreeType(&library))
        message_box("Error", "Failed to initialize Freetype", true);

    std::string font_path = (config.option<bool>("use_font_path") ? 
            config.option<std::string>("font_path") : 
            FontFace::get_system_font(config.option<std::string>("font_family")));

    FontFace font{ library, font_path,
                static_cast<unsigned int>(config.option<int>("font_size")),
                config.option<int>("tab_spaces"), };

    Renderer renderer{ font, config.option<int>("bg_color")};

    TextEngine engine{ renderer, font,
                config.option<int>("font_size"),
                config.option<int>("bg_color"),
                config.option<int>("fg_color"),
                config.option<int>("cl_color"),
                config.option<int>("cr_color"),
                config.option<int>("ln_color"),
                config.option<int>("gt_color"),
                config.option<bool>("hl_cur_line"),
                config.option<bool>("line_numbers"),
                config.option<bool>("block_caret"), };

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        engine.process_input(window);

        while (!buffer.empty())
        {
            engine.append(buffer.front());
            buffer.pop();
        }

        engine.render();

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    FT_Done_FreeType(library);
    glfwTerminate();

    return EXIT_SUCCESS;
}

void key_callback(GLFWwindow*, int key, int , int action, int)
{
    if (action == GLFW_REPEAT || action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_BACKSPACE:
                buffer.push('\b');
                break;

            case GLFW_KEY_TAB:
                buffer.push('\t');
                break;

            case GLFW_KEY_ENTER:
                buffer.push(GLFW_KEY_ENTER);
                break;

            case GLFW_KEY_UP:
                buffer.push(GLFW_KEY_UP);
                break;

            case GLFW_KEY_DOWN:
                buffer.push(GLFW_KEY_DOWN);
                break;

            case GLFW_KEY_LEFT:
                buffer.push(GLFW_KEY_LEFT);
                break;

            case GLFW_KEY_RIGHT:
                buffer.push(GLFW_KEY_RIGHT);
                break;

            case GLFW_KEY_DELETE:
                buffer.push(GLFW_KEY_DELETE);
                break;
        }
    }
}

void scroll_callback(GLFWwindow*, double, double yoffset)
{
    // Magic number to signal a scroll event
    buffer.push((static_cast<int>(yoffset) == 1) ? 321 : 320);
}

void character_callback(GLFWwindow*, unsigned int codepoint)
{
    buffer.push(codepoint);
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}
