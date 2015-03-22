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

//Als Referenz verwendete Websites:
//https://open.gl/feedback
//http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=26

//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//C++
#include <iostream>
#include <cmath>
#include <iomanip>

//Eigene Klassen
#include "shader.h"
#include "shaderprogram.h"
#include "buffer.h"
#include "vertexarray.h"
#include "openglerror.h"

//Geklauter und dann bearbeiteter Quelltext:
#include "openglerrorcallback.h"

struct treeVertex {
	glm::vec3 position;
	float length;

	treeVertex(float x, float y, float z, float length)
	{
		position = glm::vec3(x,y,z);
		this->length = length;
	}
};

int nTriangles(int numberOfIterations);
int nVertices(int numberOfIterations);

int main(void)
{
	const glm::i32vec2 startResolution(800,600);
	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	window = glfwCreateWindow(startResolution.x, startResolution.y, "Procedural Tree Generation Using A Geometry Shader And Transform Feedback", NULL, NULL); // Windowed
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	registerDebugMessageCallback();

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

	//Shader zum generieren der Geometrie:
	Shader genVertexShader(ShaderType::Vertex, "data/tree.vert");
	Shader genGeometryShader(ShaderType::Geometry, "data/tree.geo");
	Shaderprogram genShaderprogram;
	genShaderprogram.attachShader(genVertexShader);
	genShaderprogram.attachShader(genGeometryShader);

	std::vector<std::string> varyings{"out_position", "out_length"};
	genShaderprogram.transformFeedbackVaryings(varyings);
	genShaderprogram.linkProgram();
	genShaderprogram.detatchShaders();

	//Shader zum rendern:
	Shader renderVertexShader(ShaderType::Vertex, "data/render.vert");
	Shader renderFragmentShader(ShaderType::Fragment, "data/render.frag");
	Shaderprogram renderShaderprogram;
	renderShaderprogram.attachShader(renderVertexShader);
	renderShaderprogram.attachShader(renderFragmentShader);
	renderShaderprogram.linkProgram();
	renderShaderprogram.detatchShaders();

	Buffer triangleVertexBuffer(GL_ARRAY_BUFFER);
	Buffer transformFeedbackBufferA(GL_ARRAY_BUFFER);

	//buffer und vao für generierung:
//	treeVertex data[3] = {
//		treeVertex(-1.0f,-1.0f,0.0f, 2.f),
//		treeVertex(1.0f,-1.0f,0.0f, 2.f),
//		treeVertex(0.0f,1.0f,0.0f, 2.f)};

	int scl = 2.0f;
	treeVertex data[3] = {
		treeVertex(-1.0f*scl,-1.0f*scl,0.0f*scl, 2.f*scl),
		treeVertex(1.0f*scl,-1.0f*scl,0.0f*scl, 2.f*scl),
		treeVertex(0.0f*scl,1.0f*scl,0.0f*scl, 2.f*scl)};

	int numberOfIterations = 6;
	std::cout << "nVertices( " << numberOfIterations << " ) = " << nVertices(numberOfIterations) << std::endl;

	triangleVertexBuffer.bufferDataStaticRead(sizeof(treeVertex) * nVertices(numberOfIterations), nullptr);
	transformFeedbackBufferA.bufferDataStaticRead(sizeof(treeVertex) * nVertices(numberOfIterations), nullptr);

	triangleVertexBuffer.subData(sizeof(data), data);

	VertexArray genVertexArray;
	GLint position_location = genShaderprogram.getAttirbLocation("position");
	GLint length_location = genShaderprogram.getAttirbLocation("length");
	genVertexArray.enableVertexAttribArray(position_location);
	genVertexArray.enableVertexAttribArray(length_location);
	genVertexArray.vertexAttribPointer(triangleVertexBuffer, position_location, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, position));
	genVertexArray.vertexAttribPointer(triangleVertexBuffer, length_location, 1,  GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, length));

	VertexArray renderVertexArray;
	GLint renderPosition_location = genShaderprogram.getAttirbLocation("position");
	GLint renderLength_location = genShaderprogram.getAttirbLocation("length");
	renderVertexArray.enableVertexAttribArray(renderPosition_location);
	renderVertexArray.enableVertexAttribArray(renderLength_location);
	renderVertexArray.vertexAttribPointer(transformFeedbackBufferA, renderPosition_location, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, position));
	renderVertexArray.vertexAttribPointer(transformFeedbackBufferA, renderLength_location, 1, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, length));

	glm::dvec2 mouseDelta;
	glm::vec3 cameraPosition(0,0,-50);

	float modelRotaitonX = 0.0f;
	float modelRotationY = 0.0f;

	VertexArray* currentVertexArray = &genVertexArray;
	Buffer* currentTransformFeedbackBuffer = &transformFeedbackBufferA;

	VertexArray* lastVertexArray = &renderVertexArray;
	Buffer* lastTransformFeedbackBuffer = &triangleVertexBuffer;


	//Was vor den Passes in data[] liegt:
	std::cout << "Inhalt des data[] arrays." << std::endl;
	for(int i = 0; i < 3; i++) {
		int vertexStart = i * 4;
		std::cout << "Vertex " << i << "\t:("
					 << ((float*)data)[vertexStart+0] << "\t, "
					 << ((float*)data)[vertexStart+1] << "\t, "
					 << ((float*)data)[vertexStart+2] <<  "\t)\tlength = "
					 << ((float*)data)[vertexStart+3] << std::endl;
	}
	std::cout << "-----------------------------------" << std::endl;

	//Was vor den Passes im Array liegt:
	std::cout << "Inhalt des Buffers vor dem ersten Pass:" << std::endl;
	triangleVertexBuffer.bind();
	GLfloat feedback[nVertices(0) * 4];
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedback), feedback);
	triangleVertexBuffer.unbind();
	for(int i = 0; i < nVertices(0); i++) {
		int vertexStart = i * 4;
		std::cout << "Vertex " << i << "\t:("
					 << feedback[vertexStart+0] << "\t, "
					 << feedback[vertexStart+1] << "\t, "
					 << feedback[vertexStart+2] <<  "\t)\tlength = "
					 << feedback[vertexStart+3] << std::endl;
	}
	std::cout << "-----------------------------------" << std::endl;

	//Die mehreren Passes:
	for(int pass = 0; pass < numberOfIterations; pass++) {
		currentVertexArray->bind(); //das VertexArray weiß selbst aus welchem Buffer es die Daten lesen soll.
		genShaderprogram.beginUsingProgram();

		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, currentTransformFeedbackBuffer->getBuffer());
		glBeginTransformFeedback(GL_TRIANGLES);

		glDrawArrays(GL_TRIANGLES, 0, nVertices(pass));

		glEndTransformFeedback();
		glFlush();
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

		currentTransformFeedbackBuffer->bind();
		GLfloat feedback[nVertices(pass+1) * 4];
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedback), feedback);
		currentTransformFeedbackBuffer->unbind();

		std::cout << "Inhalt des Buffers nach dem " << pass+1 << "ten pass:" << std::endl;
		for(int i = 0; i < nVertices(pass+1); i++) {
			int vertexStart = i * 4;
			std::cout << "Vertex " << i << "\t:("
						 << feedback[vertexStart+0] << "\t, "
						 << feedback[vertexStart+1] << "\t, "
						 << feedback[vertexStart+2] <<  "\t)\tlength = "
						 << feedback[vertexStart+3] << std::endl;
		}
		std::cout << "-----------------------------------" << std::endl;

		genShaderprogram.stopUsingProgram();

		currentVertexArray->unbind();

		//Buffer und Vertexarrays durchtauschen
		VertexArray* swapVertexArray = currentVertexArray;
		Buffer* swapTransformFeedbackBuffer = currentTransformFeedbackBuffer;
		currentVertexArray = lastVertexArray;
		currentTransformFeedbackBuffer = lastTransformFeedbackBuffer;
		lastVertexArray = swapVertexArray;
		lastTransformFeedbackBuffer = swapTransformFeedbackBuffer;
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Update
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		glm::dvec2 mousePosition;
		glfwGetCursorPos(window, &mousePosition.x, &mousePosition.y);
		glm::dvec2 screenCenter(windowWidth/2.f, windowHeight/2.f);
		mouseDelta = mousePosition - screenCenter;
		glfwSetCursorPos(window, screenCenter.x, screenCenter.y);
		if(mouseDelta.x != 0 || mouseDelta.y != 0 ) {
			std::cout << "MouseX: " << mouseDelta.x << "  MouseY: " << mouseDelta.y << std::endl;
		}

		//Model Matix
		glm::mat4 model  = glm::mat4(1.0f);
		modelRotaitonX += mouseDelta.y * 0.01;
		modelRotationY += mouseDelta.x * 0.01;
		model = glm::rotate(model, modelRotaitonX, glm::vec3(1.0f,0.0f,0.0f));
		model = glm::rotate(model, modelRotationY, glm::vec3(0.0f,1.0f,0.0f));

		//View Matrix
		glm::mat4 view = glm::lookAt(
					cameraPosition,
					glm::vec3(0.0f,0.0f,0.0f), // and looks at the origin
					glm::vec3(0,1,0)
					);



		glm::mat4 projection = glm::perspective(
					50.f,
					(float) windowWidth / (float)windowHeight,
					0.1f,
					100.0f
					);

		glm::mat4 MVP = projection * view * model;

		renderShaderprogram.setUniform(std::string("MVP"), MVP);

		//RENDER:
		currentVertexArray->bind();
		renderShaderprogram.beginUsingProgram();

		glDrawArrays(GL_TRIANGLES, 0, nVertices(numberOfIterations));

		renderShaderprogram.stopUsingProgram();
		currentVertexArray->unbind();

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

/**
 * @brief nTriangles gibt für eine Anzahl \a numberOfIterations an Interationen
 * durch den GeometryShader tree.geo die Anzahl an erzeugten Dreiecken zurück.
 * @param numberOfIterations muss ein Integer größer oder gleich 0 sein.
 * @return
 */
int nTriangles(int numberOfIterations) {
	assert (numberOfIterations >= 0);
	return 4*std::pow(3,numberOfIterations) - 3;
}

int nVertices(int numberOfIterations) {
	return nTriangles(numberOfIterations) * 3;
}
