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

void Shaderprogram::linkProgram()
{
	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	assert(linkStatus == GL_TRUE);
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
	return glGetAttribLocation(program, attirbuteName.c_str());
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
	glUniform2fv(location, sizeof(glm::vec2), (GLfloat*) &value);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, glm::vec3 const &value)
{
	glUseProgram(program);
	glUniform3fv(location, sizeof(glm::vec3), (GLfloat*) &value);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, glm::vec4 const &value)
{
	glUseProgram(program);
	glUniform4fv(location, sizeof(glm::vec4), (GLfloat*) &value);
	glUseProgram(0);
}

//MATRIX
void Shaderprogram::setUniform(GLint location, glm::mat3 const &value)
{
	glUseProgram(program);
	glUniformMatrix3fv(location, sizeof(glm::mat3), false, (GLfloat*) &value);
	glUseProgram(0);
}

void Shaderprogram::setUniform(GLint location, glm::mat4 const &value)
{
	glUseProgram(program);
	glUniformMatrix4fv(location, sizeof(glm::mat4), false, (GLfloat*) &value);
	glUseProgram(0);
}

void Shaderprogram::setUniform(std::string uniformName, float value)
{
	GLint uniformLocation = getUniformLocation(uniformName);
	setUniform(uniformLocation, value);
}
