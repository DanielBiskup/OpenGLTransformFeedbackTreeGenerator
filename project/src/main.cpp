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

/**
 *  \brief     Hauptprogramm
 *  \details   Das Hauptprogramm mit dem main loop und der erzeugung des OpenGL Context und Fensters
 *  \author    Daniel Biskup
 *  \version   1.0
 *  \date      2015
 *  \copyright BSD 3-Clause
 */

//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//C++
#include <iostream>

//Eigene Klassen
#include "shader.h"
#include "shaderprogram.h"
#include "buffer.h"
#include "vertexarray.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(800, 600, "Procedural Tree Generation Using A Geometry Shader And Transform Feedback", NULL, NULL); // Windowed
	//window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), NULL); // Fullscreen

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//GLEW aufrufen damit es uns die Funktionpointer auf OpenGL-Funktionen zur Verfügung stellt.
	glewExperimental = GL_TRUE;
	glewInit(); //Darf erst aufgerufen weden wenn ein context gerade current ist.

	//Set some OpenGL states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.4f, 0.2f, 0.0f);

	//Ein paar Variablen sammeln die sich vermutlich nicht ändern werden:
	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	int desktopHeight = mode->height;
	int desktopWidth = mode->width;

	//Fenster in die Mitte des Bildschirms setzen:
	int windowWidth;
	int windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glfwSetWindowPos(window, (desktopWidth-windowWidth)/2, (desktopHeight-windowHeight)/2);

	//TEST:
	Shader vertexShader(ShaderType::Vertex, "data/tree.vert");
	Shader fragmentShader(ShaderType::Fragment, "data/tree.frag");
	Shaderprogram shaderprogram;
	shaderprogram.attachShader(vertexShader);
	shaderprogram.attachShader(fragmentShader);
	shaderprogram.linkProgram();
	shaderprogram.detatchShaders();

//	shaderprogram.setUniform("objectColor", adawda);

	Buffer vertexBuffer(GL_ARRAY_BUFFER);

	glm::vec3 data[3];
	data[0] = glm::vec3(-1.0f,-1.0f,0.0f);
	data[1] = glm::vec3(1.0f,-1.0f,0.0f);
	data[2] = glm::vec3(0.0f,1.0f,0.0f);

//	GLfloat data[] = {
//	   -1.0f, -1.0f, 0.0f,
//	   1.0f, -1.0f, 0.0f,
//	   0.0f,  1.0f, 0.0f,
//	};

	vertexBuffer.bufferData(sizeof(data), data);

	VertexArray vertexArray;

	GLint position_location = shaderprogram.getAttirbLocation("position");
	vertexArray.enableVertexAttribArray(position_location);
	vertexArray.vertexAttribPointer(vertexBuffer, position_location, 3, GL_FLOAT, GL_FALSE, 0 ,0 );

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		//Test draw:
		vertexArray.bind();
		//vertexBuffer.bind(); //das VertexArray weiß selbst aus welchem Buffer es die Daten lesen soll.
		shaderprogram.beginUsingProgram();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		shaderprogram.stopUsingProgram();
		//vertexBuffer.unbind();
		vertexArray.unbind();

		glm::mat4 model  = glm::mat4(1.0f);

		glm::vec3 cameraPosition(0,0,10);
		glm::mat4 view = glm::lookAt(
					cameraPosition,
					glm::vec3(0,0,0), // and looks at the origin
					glm::vec3(0,1,0)
					);

		glm::mat4 projection = glm::perspective(
					50.f,
					(float) windowWidth / (float)windowHeight,
					0.1f,
					100.0f
					);
		glm::mat4 MVP = projection * view * model;

		shaderprogram.setUniform(std::string("MVP"), MVP);


		//glm::mat4 VP = projection * view;




		glm::vec3 lightPosition_worldSpace(0,0,1.5);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
		{
			glfwSetWindowSize(window, desktopWidth, desktopHeight);
			glfwSetWindowPos(window,0,-20);
		}
	}

	glfwTerminate();
	return 0;
}
