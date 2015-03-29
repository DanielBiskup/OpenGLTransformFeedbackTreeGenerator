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

#include "shaderprogram.h"
#include <assert.h>
#include "shader.h"
#include <iostream>

Shaderprogram::Shaderprogram()
{
	program = glCreateProgram();
}

Shaderprogram::~Shaderprogram()
{
	glDeleteProgram(program);
}

void Shaderprogram::attachShader(Shader shader)
{
	assert(shader.isCompiled()); //es dürfen nur compilierte Shader übergeben werden.
	glAttachShader(program, shader.getGLuint());
}

void Shaderprogram::transformFeedbackVaryings(std::vector<std::string> varyings)
{
	//from: http://stackoverflow.com/a/7048898/1510873
	std::vector<const char *> cStrArray;
	cStrArray.reserve(varyings.size());

	for(std::vector<std::string>::iterator it = varyings.begin(); it != varyings.end(); ++it)
	{
		cStrArray.push_back(((std::string)*it).c_str());
	}

	//Möglicher Fehler: Ich übergebe hier einen Pointer auf ein lokale Variable.
	glTransformFeedbackVaryings(program, cStrArray.size(), &cStrArray[0], GL_INTERLEAVED_ATTRIBS);
}

void Shaderprogram::linkProgram()
{
	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	assert(linkStatus == GL_TRUE);
	//TODO: Linkfehlerausgabe.
}

void Shaderprogram::detatchShaders()
{
	//Clean Up: detatch shaders:
	GLuint attachedShaders[10];
	int count;
	glGetAttachedShaders(program, 10, &count, attachedShaders);
	for(int i = 0; i < count; i++)
	{
		glDetachShader(program, attachedShaders[i]);
	}
}

void Shaderprogram::beginUsingProgram()
{
	glUseProgram(program);
}

void Shaderprogram::stopUsingProgram()
{
	glUseProgram(0);
}

GLint Shaderprogram::getUniformLocation(std::string uniformName)
{
	return glGetUniformLocation(program, uniformName.c_str());
}

GLint Shaderprogram::getAttirbLocation(std::string attirbuteName)
{
	GLint attribLocation = glGetAttribLocation(program, attirbuteName.c_str());
	//assert(attribLocation != -1);
	return attribLocation;
}

//FLOAT
void Shaderprogram::setUniform(GLint location, float v0)
{
	glUseProgram(program);
	glUniform1f(location, v0);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, float v0, float v1)
{
	glUseProgram(program);
	glUniform2f(location, v0, v1);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, float v0, float v1, float v2)
{
	glUseProgram(program);
	glUniform3f(location, v0, v1, v2);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, float v0, float v1, float v2, float v3)
{
	glUseProgram(program);
	glUniform4f(location, v0, v1, v2, v3);
	glUseProgram(0);
}

//VECTOR
void Shaderprogram::setUniform(GLint location, glm::vec2 const &value)
{
	glUseProgram(program);
	glUniform2fv(location, 1, (GLfloat*) &value);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, glm::vec3 const &value)
{
	glUseProgram(program);
	glUniform3fv(location, 1, (GLfloat*) &value);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, glm::vec4 const &value)
{
	glUseProgram(program);
	glUniform4fv(location, 1, (GLfloat*) &value);
	glUseProgram(0);
}

//MATRIX
void Shaderprogram::setUniform(GLint location, glm::mat3 const &value)
{
	glUseProgram(program);
	glUniformMatrix3fv(location, 1, false, (GLfloat*) &value);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, glm::mat4 const &value)
{
	glUseProgram(program);
	glUniformMatrix4fv(location, 1, false, (GLfloat*) &value);
	glUseProgram(0);
}

GLuint Shaderprogram::getProgram() {
	return program;
}
