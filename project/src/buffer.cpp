#include "buffer.h"
#include "assert.h"
#include <iostream>
#include <string>

#include "openglerror.h"

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

void Buffer::bufferDataStaticDraw(GLsizeiptr size, const GLvoid *data)
{
	bind();
	glBufferData(target, size, data, GL_STATIC_DRAW);
	unbind();
}

void Buffer::bufferDataStaticRead(GLsizeiptr size, const GLvoid *data)
{
	bind(); glErr();


	glBufferData(target, size, data, GL_STATIC_READ);
	unbind();
}

GLuint Buffer::getBuffer()
{
	return buffer;
}

//unnötig
void Buffer::beginTransformFeedback()
{
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);
	glBeginTransformFeedback(GL_TRIANGLES);
}

//unnötig
void endTransformFeedback()
{
	glEndTransformFeedback();
	glFlush();
}


