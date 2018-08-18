#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "rect.h"

class Renderer
{
public:
    Renderer();
	~Renderer();
    void draw_character(GLFWwindow *window, char c, unsigned int x, unsigned int y, const class FontFace &font);
    void draw_text(GLFWwindow *window, const std::string& text, unsigned int x, unsigned int y, const FontFace& font);

private:
	GLuint compile_shader(const char *source, GLenum shader_type);

    GLint scale_uniform_location, offset_uniform_location;
};

#endif
