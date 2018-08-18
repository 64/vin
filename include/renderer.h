#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "rect.h"
#include "fontface.h"

class Renderer
{
public:
    Renderer(const FontFace& font);
	~Renderer();
    void draw_character(GLFWwindow *window, char c, unsigned int x, unsigned int y);
    void draw_text(GLFWwindow *window, const std::string& text, unsigned int x, unsigned int y);

private:
	GLuint compile_shader(const char *source, GLenum shader_type);
    GLint scale_uniform_location, offset_uniform_location;
    const FontFace& font_face;
};

#endif
