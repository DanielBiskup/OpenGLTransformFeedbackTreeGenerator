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

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader;

class Shaderprogram
{
public:
	Shaderprogram();
	~Shaderprogram();

	/**
	 * @brief attachShader
	 * @param shader Ein Objekt der Klasse Shader
	 * @pre Der übergebene Shader muss bereits kompiliert sein.
	 */
	void attachShader(Shader shader);
	void linkProgram();
	void transformFeedbackVaryings(std::vector<std::string> varyings);
	void detatchShaders();
	void beginUsingProgram();
	void stopUsingProgram();

	GLint getUniformLocation(std::string uniformName);
	GLint getAttirbLocation(std::string attirbuteName);

	//Uniforms
	//FLOAT
	void setUniform(GLint location, float v0);
	void setUniform(GLint location, float v0, float v1);
	void setUniform(GLint location, float v0, float v1, float v2);
	void setUniform(GLint location, float v0, float v1, float v2, float v3);

	//VECTOR
	void setUniform(GLint location, glm::vec2 const &value);
	void setUniform(GLint location, glm::vec3 const &value);
	void setUniform(GLint location, glm::vec4 const &value);

	//MATRIX
	void setUniform(GLint location, glm::mat3 const &value);
	void setUniform(GLint location, glm::mat4 const &value);

	/**
	 * @brief setUniform erwartet den Namen einer Uniformvariable und den Wert auf den diese gesetzt werden soll.
	 * @param uniformName
	 * @param t
	 */
	template <typename T>
	void setUniform(std::string uniformName, T &t)
	{
		GLint uniformLocation = getUniformLocation(uniformName);
		setUniform(uniformLocation, t);
	}

	GLuint getProgram();
private:
	GLuint program;
};

#endif // SHADERPROGRAM_H
