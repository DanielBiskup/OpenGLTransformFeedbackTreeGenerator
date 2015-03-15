#ifndef BUFFER_H
#define BUFFER_H

#include "GL/glew.h"

class Buffer
{
public:
	Buffer(GLenum target);
	~Buffer();
	void bind();
	void unbind();
	void bufferData(GLsizeiptr size, const GLvoid *data);

private:
	GLuint buffer;
	GLenum target;
};

#endif // BUFFER_H
