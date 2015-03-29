/*
Copyright (c) 2015 Daniel Biskup
All rights reserved.

Redistribution and use in source and binary forms, with
or without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

2. Redistributions in binary form must reproduce the
above copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

3. Neither the name of the copyright holder nor the names
of its contributors may be used to endorse or promote
products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

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
	bind();
	glBufferData(target, size, data, GL_STATIC_READ);
	unbind();
}

void Buffer::subData(GLsizeiptr size, const GLvoid * data)
{
	bind();
	glBufferSubData(target, 0, size, data);
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


