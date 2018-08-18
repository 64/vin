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
    Renderer(const FontFace& font, const int& width, const int& height);
	~Renderer();
    long draw_character(char c, int x, int y);
    void draw_text(const std::string& text, int x, int y);

private:
	GLuint compile_shader(const char *source, GLenum shader_type);
    GLint scale_uniform_location, offset_uniform_location;
    const FontFace& font_face;
    const int& screen_width;
    const int& screen_height;
};

#endif
