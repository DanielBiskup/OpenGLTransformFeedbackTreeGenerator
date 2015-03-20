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
	void bufferDataStaticDraw(GLsizeiptr size, const GLvoid *data);
	void bufferDataStaticRead(GLsizeiptr size, const GLvoid *data);
	void subData(GLsizeiptr size, const GLvoid * data);
	GLuint getBuffer();

	void beginTransformFeedback();
	void endTransformFeedback();

private:
	GLuint buffer;
	GLenum target;
};

#endif // BUFFER_H
