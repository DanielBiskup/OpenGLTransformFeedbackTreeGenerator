/*
Copyright (c) 2015, Daniel Biskup
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	1) Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	2) Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	3) Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "shader.h"

// GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// C++
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <iostream>
#include <exception>
#include <stdexcept>

Shader::Shader(ShaderType::shader_type_t shaderType, std::string const & fileName)
{
	createShaderFromFile(shaderType, fileName);
}

Shader::~Shader()
{
	//Das geht so nicht ganz auf, denn nur weil das Shader C++ Object zerstört wird (ende des Scopes z.B.) könnte der Shader doch weiterleben, z.B. wenn er an
	//ein ShaderProgram gebunden ist. Warte... Doch kein Problem, denn in der Dokumentation von OpenGL steht:
	//"If a shader object to be deleted is attached to a program
	//object, it will be flagged for deletion, but it will not be
	//deleted until it is no longer attached to any program object,
	//for any rendering context (i.e., it must be detached from
	//wherever it was attached before it will be deleted)."
	//  https://www.khronos.org/opengles/sdk/docs/man/xhtml/glDeleteShader.xml
	glDeleteShader(shader);
}

void Shader::createShaderFromFile(ShaderType::shader_type_t shaderType, std::string const &fileName)
{
	std::ifstream t(fileName.c_str());
	if(t.is_open() == false) throw std::runtime_error("Konnte folgender Shader-Quelldatei nicht finden: " + fileName);

	std::string sourceCode((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());
	createShaderFromString(shaderType, sourceCode);
}

void Shader::createShaderFromString(ShaderType::shader_type_t shaderType, std::string const &sourceCode)
{
	char* cStyleSourceCode = (char*)sourceCode.c_str();

	this->shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &cStyleSourceCode, NULL);
	glCompileShader(shader);

	//Error checking
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(success == GL_FALSE)
	{
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> errorLog(logSize);
		glGetShaderInfoLog(shader, logSize, NULL, &errorLog[0]);
		std::string errorMessage = std::string(&errorLog[0]);
		glDeleteShader(shader);
		throw ShaderCompilationException(errorMessage);
	}
}

bool Shader::isCompiled() const
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	return isCompiled;
}

GLuint Shader::getGLuint() const
{
	return shader;
}
