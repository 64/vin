#include "renderer.h"
#include "font.h"

#include <iostream>

constexpr size_t LOG_LENGTH = 512;

const char *vert_source =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"out vec2 TexCoords;\n"
"\n"
"uniform vec2 scale;\n"
"uniform vec2 offset;\n"
"\n"
"void main()\n"
"{\n"
"    TexCoords = vec2(aPos.x, -aPos.y);\n"
"    gl_Position = vec4((aPos * scale) + offset, 0.0, 1.0);\n"
"}";

// 0.0, 0.0 -> 0.0, 0.0
// 0.0, -1.0 -> 0.0, 1.0

const char *frag_source =
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 FragColor;\n"
"\n"
"uniform sampler2D text;\n"
"void main()\n"
"{\n"
"    vec4 alpha_sample = vec4(1.0f, 1.0f, 1.0f, texture(text, TexCoords).r);\n"
"    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * alpha_sample;\n"
"}";

const float quad_vertices[] = {
    0.0f, -1.0f,
    0.0f,  0.0f,
    1.0f,  0.0f,
    1.0f,  0.0f,
    1.0f, -1.0f,
    0.0f, -1.0f
};

Renderer::Renderer()
{
    // TODO: OpenGL error checking with glGetError
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

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
        glGetProgramInfoLog(program, LOG_LENGTH, NULL, info_log);
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    scale_uniform_location = glGetUniformLocation(program, "scale");
    offset_uniform_location = glGetUniformLocation(program, "offset");

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer()
{

}

void Renderer::draw_character(GLFWwindow *window, char c, unsigned int x, unsigned int y, const Font& font)
{
    // TODO: Cache width, height
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    Glyph glyph = font.get_glyph(c);

    float xratio = (float)x / (float)width;
    float yratio = (float)y / (float)height;
    float x_offset = (xratio - 0.5f) * 2;
    float y_offset = -(yratio - 0.5f) * 2; // Flip y-axis so origin is top left

    float x_scale = (float)glyph.width / (float)width * 2;
    float y_scale = (float)glyph.height / (float)height * 2;

    glUniform2f(scale_uniform_location, x_scale, y_scale); // TODO: Cache this since it rarely changes
    glUniform2f(offset_uniform_location, x_offset, y_offset);
    
    glBindTexture(GL_TEXTURE_2D, glyph.texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::draw_text(GLFWwindow *window, const std::string& text, unsigned int x, unsigned int y, const Font& font)
{
    // TODO: Cache width, height
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    for (const auto ch : text)
    {
        Glyph glyph = font.get_glyph(ch);
        int xpos = x + glyph.bearingx;
        int ypos = y + (glyph.height - glyph.bearingy);

        float xratio = (float)xpos / (float)width;
        float yratio = (float)ypos / (float)height;
        float x_offset = (xratio - 0.5f) * 2;
        float y_offset = -(yratio - 0.5f) * 2; // Flip y-axis so origin is top left

        float x_scale = (float)glyph.width / (float)width * 2;
        float y_scale = (float)glyph.height / (float)height * 2;

        glUniform2f(scale_uniform_location, x_scale, y_scale); // TODO: Cache this since it rarely changes
        glUniform2f(offset_uniform_location, x_offset, y_offset);

        glBindTexture(GL_TEXTURE_2D, glyph.texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (glyph.advance >> 6);
    }
}

GLuint Renderer::compile_shader(const char *source, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint result;
    char info_log[LOG_LENGTH];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shader, LOG_LENGTH, NULL, info_log);
        std::cerr << "Failed to compile " << (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") << " shader:" << std::endl;
        std::cerr << info_log << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return shader;
}
