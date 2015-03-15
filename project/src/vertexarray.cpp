#include "vertexarray.h"

#include "buffer.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vertexArray);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vertexArray);
}

void VertexArray::bind()
{
	glBindVertexArray(vertexArray);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::enableVertexAttribArray(GLuint index)
{
	bind();
	glEnableVertexAttribArray(index);
	unbind();
}

void VertexArray::vertexAttribPointer(Buffer &buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
	buffer.bind();
	bind();
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	unbind();
	buffer.unbind();
}
