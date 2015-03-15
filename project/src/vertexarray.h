#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include "buffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void bind();
	void unbind();
	void enableVertexAttribArray(GLuint index);
	void vertexAttribPointer(Buffer &buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);

private:
	GLuint vertexArray;
};

#endif // VERTEXARRAY_H
