#include "renderer.h"

#include <iostream>

constexpr size_t LOG_LENGTH = 512;

const char *vert_source =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"}";

const char *frag_source =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";


Renderer::Renderer()
{
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
}

Renderer::~Renderer()
{

}

void Renderer::draw()
{

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
