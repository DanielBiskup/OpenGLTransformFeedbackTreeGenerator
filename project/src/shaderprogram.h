#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

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

	//TODO: eine Template Funktion für die hier schreiben.
	void setUniform(std::string, float value);

private:
	GLuint program;
};

#endif // SHADERPROGRAM_H
