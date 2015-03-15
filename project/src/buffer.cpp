#include "buffer.h"

Buffer::Buffer(GLenum target) : target(target)
{
	glGenBuffers(1, (GLuint*) &buffer);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, (GLuint*) &buffer);
}

void Buffer::bind()
{
	glBindBuffer(target, buffer);
}

void Buffer::unbind()
{
	glBindBuffer(target, 0);
}

void Buffer::bufferData(GLsizeiptr size, const GLvoid *data)
{
	bind();
	glBufferData(target, size, data, GL_STATIC_DRAW);
	unbind();
}
