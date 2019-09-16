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

/**
 *  \brief     Wrapper-Klasse für Shader
 *  \details   Hilft beim erzeugen und compilieren von Shadern aus GLSL-Quelldateien.
 *  \author    Daniel Biskup
 *  \version   1.0
 *  \date      2015
 *  \copyright BSD 3-Clause
 */

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <exception>
#include <stdexcept>

namespace ShaderType
{
enum shader_type_t
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER
};
}

class ShaderCompilationException : public std::runtime_error
{
public:
	ShaderCompilationException(std::string const &message) : std::runtime_error(message) {}
	~ShaderCompilationException() throw() {}
};

class Shader
{

public:
	/**
	 * @brief Shader
	 * @param shaderType
	 * @param fileName
	 * @pre   Ein OpenGL-Context muss aktiv sein.
	 */
	Shader(ShaderType::shader_type_t shaderType, std::string const &fileName);
	~Shader();
	bool isCompiled() const;
	GLuint getGLuint() const;
private:
	GLuint shader; //< Die Shader ID des gewrappten Shader Objekts.
	void createShaderFromFile(ShaderType::shader_type_t shaderType, std::string const &fileName);
	void createShaderFromString(ShaderType::shader_type_t shaderType, std::string const &sourceCode);
};

#endif // SHADER_H
