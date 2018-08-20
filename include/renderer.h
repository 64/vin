#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "fontface.h"
#include "util.h"

class Renderer
{
public:
    Renderer(const FontFace& font, int bg_color);
	~Renderer();
    long draw_character(unsigned char c, Vec2i pos, const Vec3f& color);
    Vec2i draw_text(const std::string& text, Vec2i pos, const Vec3f& color, bool ln);
    void draw_rectangle(IntRect pos, const Vec3f& color);

private:
    GLuint compile_shader(const char *source, GLenum shader_type);
    GLint scale_uniform_location, offset_uniform_location, color_uniform_location;
    const FontFace& font_face;
    Vec3f bg_color;
};

#endif
