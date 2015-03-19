#include "buffer.h"
#include "assert.h"
#include <iostream>
#include <string>
std::string errorString(GLenum error);

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
	bind();
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		std::cout << errorString(error);
	}
	assert(error == GL_NO_ERROR);

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

std::string errorString(GLenum error)
{
	switch(error){
	case GL_NO_ERROR:
		return std::string("No error has been recorded. "
						   "The value of this symbolic constant is guaranteed to be 0.");
		break;
	case GL_INVALID_ENUM:
		return std::string("An unacceptable value is specified for an enumerated argument."
						   "The offending command is ignored and has no other side effect "
						   "than to set the error flag.");
		break;
	case GL_INVALID_VALUE:
		return std::string("A numeric argument is out of range."
						   "The offending command is ignored and has no other side effect"
						   "than to set the error flag.");
		break;
	case GL_INVALID_OPERATION:
		return std::string("The specified operation is not allowed in the current state."
						   "The offending command is ignored and has no other side effect"
						   "than to set the error flag.");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
			return std::string("The framebuffer object is not complete. The offending command"
							   " is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_OUT_OF_MEMORY:
			return std::string("There is not enough memory left to execute the command."
							   "The state of the GL is undefined, except for the state "
							   "of the error flags after this error is recorded.");
		break;
	case GL_STACK_UNDERFLOW:
			return std::string("An attempt has been made to perform an operation that would"
							   "cause an internal stack to underflow.");
		break;
	case GL_STACK_OVERFLOW:
			return std::string("An attempt has been made to perform an operation that would"
							   "cause an internal stack to overflow.");
		break;
	default:
		return std::string("Unbekannter Fehler");
	}
}
