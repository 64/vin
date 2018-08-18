#include <iostream>

#include "renderer.h"
#include "fontface.h"
#include "util.h"

constexpr size_t LOG_LENGTH = 512;

static const char *vert_source =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"out vec2 TexCoords;\n"
"\n"
"uniform vec2 scale;\n"
"uniform vec2 offset;\n"
"\n"
"void main()\n"
"{\n"
"    TexCoords = vec2(aPos.x, 1 - aPos.y);\n"
"    gl_Position = vec4((aPos * scale) + offset, 0.0, 1.0);\n"
"}";

static const char *frag_source =
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 FragColor;\n"
"\n"
"uniform sampler2D text;\n"
"uniform vec3 color;\n"
"void main()\n"
"{\n"
"    vec4 alpha_sample = vec4(1.0f, 1.0f, 1.0f, texture(text, TexCoords).r);\n"
"    FragColor = vec4(color, 1.0f) * alpha_sample;\n"
"}";

const float quad_vertices[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

Renderer::Renderer(const FontFace& font, const int& width, const int& height, int _bg_color, int _fg_color)
    : font_face(font), screen_width(width), screen_height(height), bg_color(rgb_to_vec(_bg_color)), fg_color(rgb_to_vec(_fg_color))
{
    // TODO: OpenGL error checking with glGetError
    glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.0f);

    GLuint frag_shader = compile_shader(frag_source, GL_FRAGMENT_SHADER);
    GLuint vert_shader = compile_shader(vert_source, GL_VERTEX_SHADER);

    GLint result;
    char info_log[LOG_LENGTH];
    GLuint program = glCreateProgram();
    glAttachShader(program, frag_shader);
    glAttachShader(program, vert_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(program, LOG_LENGTH, nullptr, info_log);
        std::cerr << "Failed to link shader program:" << std::endl;
        std::cerr << info_log << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glUseProgram(program);
    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    scale_uniform_location = glGetUniformLocation(program, "scale");
    offset_uniform_location = glGetUniformLocation(program, "offset");
    color_uniform_location = glGetUniformLocation(program, "color");

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer()
{

}

long Renderer::draw_character(char c, int x, int y)
{
    Glyph glyph = font_face.get_glyph(c);
    int xpos = x + glyph.bearingx;
    int ypos = y + glyph.height - glyph.bearingy;

    float xratio = static_cast<float>(xpos) / static_cast<float>(screen_width);
    float yratio = static_cast<float>(ypos) / static_cast<float>(screen_height);
    float x_offset = (xratio - 0.5f) * 2;
    float y_offset = -(yratio - 0.5f) * 2; // Flip y-axis so origin is top left

    float x_scale = static_cast<float>(glyph.width) / static_cast<float>(screen_width) * 2;
    float y_scale = static_cast<float>(glyph.height) / static_cast<float>(screen_height) * 2;

    glUniform2f(scale_uniform_location, x_scale, y_scale); // TODO: Cache this since it rarely changes
    glUniform2f(offset_uniform_location, x_offset, y_offset);
    glUniform3f(color_uniform_location, fg_color.r, fg_color.g, fg_color.b);
    
    glBindTexture(GL_TEXTURE_2D, glyph.texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return glyph.advancex;
}

void Renderer::draw_text(const std::string& text, int x, int y)
{
    int old_x = x;
    for (const auto ch : text)
    {
        if (ch == '\n')
        {
            y += font_face.font_height();
            x = old_x; // Temp until TextEngine is functional
            continue;
        }

        x += (draw_character(ch, x, y) >> 6);
    }
}

GLuint Renderer::compile_shader(const char *source, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint result;
    char info_log[LOG_LENGTH];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shader, LOG_LENGTH, nullptr, info_log);
        std::cerr << "Failed to compile " << (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") << " shader:" << std::endl;
        std::cerr << info_log << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return shader;
}
