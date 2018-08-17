#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <string>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void draw();
private:
	GLuint compile_shader(const char *source, GLenum shader_type);
};

#endif
