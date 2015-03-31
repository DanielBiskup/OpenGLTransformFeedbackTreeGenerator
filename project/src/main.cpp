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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//AntTeakBar
#include <AntTweakBar.h>

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
#include "tweakbarutil.h"

//Geklauter und dann bearbeiteter Quelltext:
#include "openglerrorcallback.h"

struct treeVertex {
	glm::vec3 position;
	float length;
	glm::vec3 normal;

	treeVertex(float x, float y, float z, float length)
	{
		position = glm::vec3(x,y,z);
		this->length = length;
		this->normal = glm::vec3(0.0f, 1.0f, 0.0f);
	}
};

struct VertexArraysAndBufers {
	VertexArray* currentVertexArray;
	Buffer* currentTransformFeedbackBuffer;
	VertexArray* lastVertexArray;
	Buffer* lastTransformFeedbackBuffer;
};

struct ButtonCallbackParameters {
	VertexArraysAndBufers* vertexArraysAndBufers;
	Shaderprogram* shader;
	int* numberOfIterations;
	int* numberOfVerticesToDraw;
	int* numberOfTrianglesToDraw;
	float* scaleLengthUniform;
	float* scaleTriangleUniform;
	float* pyramidFactorUniform;
};

int nTriangles(int numberOfIterations);
int nVertices(int numberOfIterations);
void generate(VertexArraysAndBufers& vertexArraysAndBufers, Shaderprogram& shader, int numberOfIterations);
void theGenerateButtonCallbackFunction(void *clientData);

int main(void)
{
	const glm::i32vec2 startResolution(1000,700);
	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	window = glfwCreateWindow(startResolution.x, startResolution.y, "Procedural Tree Generation Using A Geometry Shader And Transform Feedback", NULL, NULL); // Windowed

	//Sichtbarer Cursor:
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

	//Debbugcallback:
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

	//Variables exposed by UI
	//--generation
	int numberOfIterations = 7;
	float scaleLengthUniform = 0.75f;
	float scaleTriangleUniform = 0.58f;
	float pyramidFactorUniform = 0.14f;
	//--presentation
	glm::quat rotationQuaternion;
	float autoRotationSpeed = 0.1f;
	int autoRotateBoolean = 1;

	//Variables not exposed as RO by UI
	int numberOfVerticesToDraw = 0;
	const int maxNumberOfIterations = 12;

	//Variables that only exist to be displayed by the UI
	int numberOfTrianglesToDraw = 0;


	//Shader zum generieren der Geometrie:
	Shader genVertexShader(ShaderType::Vertex, "data/tree.vert");
	Shader genGeometryShader(ShaderType::Geometry, "data/tree.geo");
	Shaderprogram genShaderprogram;
	genShaderprogram.attachShader(genVertexShader);
	genShaderprogram.attachShader(genGeometryShader);

	std::vector<std::string> varyings;
	varyings.push_back("out_position");
	varyings.push_back("out_length");
	varyings.push_back("out_normal");
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

	triangleVertexBuffer.bufferDataStaticRead(sizeof(treeVertex) * nVertices(maxNumberOfIterations), 0);
	transformFeedbackBufferA.bufferDataStaticRead(sizeof(treeVertex) * nVertices(maxNumberOfIterations), 0);

	VertexArray genVertexArray;
	GLint position_location = genShaderprogram.getAttirbLocation("position");
	GLint length_location = genShaderprogram.getAttirbLocation("length");
	GLint normal_location = genShaderprogram.getAttirbLocation("normal");
	genVertexArray.enableVertexAttribArray(0);
	genVertexArray.enableVertexAttribArray(1);
	genVertexArray.enableVertexAttribArray(2);
	genVertexArray.vertexAttribPointer(triangleVertexBuffer, position_location, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, position));
	genVertexArray.vertexAttribPointer(triangleVertexBuffer, length_location, 1,  GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, length));
	genVertexArray.vertexAttribPointer(triangleVertexBuffer, normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, normal));

	VertexArray renderVertexArray;
	GLint renderPosition_location = genShaderprogram.getAttirbLocation("position");
	GLint renderLength_location = genShaderprogram.getAttirbLocation("length");
	GLint renderNormal_location = genShaderprogram.getAttirbLocation("normal");
	renderVertexArray.enableVertexAttribArray(0);
	renderVertexArray.enableVertexAttribArray(1);
	renderVertexArray.enableVertexAttribArray(2);
	renderVertexArray.vertexAttribPointer(transformFeedbackBufferA, renderPosition_location, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, position));
	renderVertexArray.vertexAttribPointer(transformFeedbackBufferA, renderLength_location, 1, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, length));
	renderVertexArray.vertexAttribPointer(transformFeedbackBufferA, renderNormal_location, 3, GL_FLOAT, GL_FALSE, sizeof(treeVertex), (GLvoid*) offsetof(treeVertex, normal));

	VertexArraysAndBufers vertexArraysAndBufers;
	vertexArraysAndBufers.currentVertexArray = &genVertexArray;
	vertexArraysAndBufers.currentTransformFeedbackBuffer = &transformFeedbackBufferA;
	vertexArraysAndBufers.lastVertexArray = &renderVertexArray;
	vertexArraysAndBufers.lastTransformFeedbackBuffer = &triangleVertexBuffer;

	//AntTweakBar
	initTweakbar(window);
	TwInit(TW_OPENGL_CORE, NULL); // for core profile
	TwWindowSize(windowWidth, windowHeight);
	TwBar *bar;
	bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='250 300' color='25 102 51' "); // change default tweak bar size and color

	ButtonCallbackParameters buttonCallbackParameters;
	buttonCallbackParameters.numberOfIterations = &numberOfIterations;
	buttonCallbackParameters.shader = &genShaderprogram;
	buttonCallbackParameters.vertexArraysAndBufers = &vertexArraysAndBufers;
	buttonCallbackParameters.numberOfVerticesToDraw = &numberOfVerticesToDraw;
	buttonCallbackParameters.numberOfTrianglesToDraw = &numberOfTrianglesToDraw;
	buttonCallbackParameters.scaleLengthUniform = &scaleLengthUniform;
	buttonCallbackParameters.scaleTriangleUniform = &scaleTriangleUniform;
	buttonCallbackParameters.pyramidFactorUniform = &pyramidFactorUniform;

	TwAddButton(bar, "Run", theGenerateButtonCallbackFunction, &buttonCallbackParameters,  " group='generation parameters' key=g label='click here to generate tree' ");
	TwAddVarRW(bar, "numberOfIterations", TW_TYPE_INT8, &numberOfIterations, "group='generation parameters' min=0 max=12");
	TwAddVarRW(bar, "scaleLength", TW_TYPE_FLOAT, &scaleLengthUniform, "group='generation parameters' min=0 max=1 step=0.01 keyIncr=l keyDecr=L help='Gibt den Factor an, um den die Laenge eines Astes aus Iteration n-1 groesser ist als die die Laenge eines Astes aus Iteration n.' ");
	TwAddVarRW(bar, "scaleTriangle", TW_TYPE_FLOAT, &scaleTriangleUniform, "group='generation parameters' min=0 max=1 step=0.01 keyIncr=l keyDecr=L help='Gibt den Factor an, um den die Laenge eines Astes aus Iteration n-1 groesser ist als die die Laenge eines Astes aus Iteration n.' ");
	TwAddVarRW(bar, "pyramidFactor", TW_TYPE_FLOAT, &pyramidFactorUniform, "group='generation parameters' min=0 max=1 step=0.01 keyIncr=l keyDecr=L help='Gibt den Factor an, um den die Laenge eines Astes aus Iteration n-1 groesser ist als die die Laenge eines Astes aus Iteration n.' ");

	TwAddVarRW(bar, "rotation", TW_TYPE_QUAT4F, &rotationQuaternion, "group='presentation parameters' opened=true");
	TwAddVarRW(bar, "autoRotation", TW_TYPE_BOOL32, &autoRotateBoolean, "group='presentation parameters' help='Gibt den Factor an, um den die Laenge eines Astes aus Iteration n-1 groesser ist als die die Laenge eines Astes aus Iteration n.' ");
	TwAddVarRW(bar, "autoRotationSpeed", TW_TYPE_FLOAT, &autoRotationSpeed, "group='presentation parameters' min=0 max=1 step=0.01 keyIncr=l keyDecr=L help='Gibt den Factor an, um den die Laenge eines Astes aus Iteration n-1 groesser ist als die die Laenge eines Astes aus Iteration n.' ");

	TwAddVarRO(bar, "number of triangles", TW_TYPE_INT32, &numberOfTrianglesToDraw, "group='read-only scene information'");
	TwAddVarRO(bar, "number of vertices", TW_TYPE_INT32, &numberOfVerticesToDraw, "group='read-only scene information'");

	//Hier wird die callback function einmal manuell aufgerufen, damit beim Start des Programmes schon
	//Geometrie auf dem Bildschirm zu sehen ist.
	theGenerateButtonCallbackFunction(&buttonCallbackParameters);

	/* Loop until the user closes the window */
	glm::dvec2 mouseDelta;
	double currentFrame, deltaTime;
	double lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		//Deltatime bestimmen:
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Update
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		glm::dvec2 mousePosition;
		//glfwGetCursorPos(window, &mousePosition.x, &mousePosition.y);
		mouseDelta = mousePosition;
		//glfwSetCursorPos(window, 0.0d, 0.0d);

		if(mouseDelta.x != 0 || mouseDelta.y != 0 ) {
			std::cout <<"mp("<<mousePosition.x<<", "<<mousePosition.y<<")" <<std::endl<< "MouseX: " << mouseDelta.x << "  MouseY: " << mouseDelta.y << std::endl;
		}

		//View Matrix
		glm::vec3 cameraPosition(0, -50, -250);
		glm::mat4 view = glm::lookAt(
					cameraPosition,
					glm::vec3(0.0f,-30.0f,0.0f), // and looks at the origin
					glm::vec3(0,1,0)
					);

		glm::mat4 projection = glm::perspective(
					50.f,
					(float)windowWidth / (float)windowHeight,
					0.1f,
					1000.0f
					);

		if(autoRotateBoolean) {
			rotationQuaternion = rotationQuaternion * glm::angleAxis(10.f * autoRotationSpeed * (float)deltaTime, glm::vec3(0,1,0));
		}
		glm::mat4 M	= glm::toMat4(rotationQuaternion);
		glm::mat4 MVP	= projection * view * M;
		glm::vec3 lightPosition(cameraPosition);

		renderShaderprogram.setUniform(std::string("MVP"), MVP);
		renderShaderprogram.setUniform(std::string("M"), M);
		renderShaderprogram.setUniform(std::string("lightposition"), lightPosition);

		//RENDER:
		vertexArraysAndBufers.currentVertexArray->bind();
		renderShaderprogram.beginUsingProgram();

		glDrawArrays(GL_TRIANGLES, 0, numberOfVerticesToDraw);

		renderShaderprogram.stopUsingProgram();
		vertexArraysAndBufers.currentVertexArray->unbind();

		TwDraw();  // draw the tweak bar(s)

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		//http://www.glfw.org/docs/latest/quick.html#quick_process_events
		//Das aufrufen dieser Funktion löst aus, dass Events bearbeitet werden, das heißt, dass die CallbackFunktionen
		//und damit auch die Reaktionen auf Events wie Buttonclicks in der AntTweakBar hier aufgerufen werden.
		//Dies ist also auch jene Stelle an der neue Geometrie erzeugt wird sobald der "generate"-Button gedrückt wird.
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

void generate(VertexArraysAndBufers& vertexArraysAndBufers, Shaderprogram& shader, int numberOfIterations) {
	float scl = 4.0f;
	float length = 4.f;
	float a = 6.0f;
	float h = std::sqrt(3) * a * 0.5f;
	float halfA = a*0.5f;

	treeVertex data[3] = {
		treeVertex(-halfA	,0.0f	,0.0f	, length*scl),
		treeVertex(halfA	,0.0f	,0.0f	, length*scl),
		treeVertex(0.0f		,0.0f	,h		, length*scl)};

	std::cout << "nVertices( " << numberOfIterations << " ) = " << nVertices(numberOfIterations) << std::endl;

	//Daten in Buffer schreiben:
	vertexArraysAndBufers.lastTransformFeedbackBuffer->subData(sizeof(data), data);

	/*
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
	vertexArraysAndBufers.lastTransformFeedbackBuffer->bind();
	GLfloat feedback[nVertices(0) * 4];
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedback), feedback);
	vertexArraysAndBufers.lastTransformFeedbackBuffer->unbind();
	for(int i = 0; i < nVertices(0); i++) {
		int vertexStart = i * 4;
		std::cout << "Vertex " << i << "\t:("
					 << feedback[vertexStart+0] << "\t, "
					 << feedback[vertexStart+1] << "\t, "
					 << feedback[vertexStart+2] <<  "\t)\tlength = "
					 << feedback[vertexStart+3] << std::endl;
	}
	std::cout << "-----------------------------------" << std::endl;
	*/

	//Die mehreren Passes:
	for(int pass = 0; pass < numberOfIterations; pass++) {
		vertexArraysAndBufers.currentVertexArray->bind(); //das VertexArray weiß selbst aus welchem Buffer es die Daten lesen soll.
		shader.beginUsingProgram();

		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vertexArraysAndBufers.currentTransformFeedbackBuffer->getBuffer());
		glBeginTransformFeedback(GL_TRIANGLES);

		glDrawArrays(GL_TRIANGLES, 0, nVertices(pass));

		glEndTransformFeedback();
		glFlush();
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

		/*
		vertexArraysAndBufers.currentTransformFeedbackBuffer->bind();
		GLfloat feedback[nVertices(pass+1) * 7];
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedback), feedback);
		vertexArraysAndBufers.currentTransformFeedbackBuffer->unbind();

		std::cout << "Inhalt des Buffers nach dem " << pass+1 << "ten pass:" << std::endl;
		for(int i = 0; i < nVertices(pass+1); i++) {
			int vertexStart = i * 7;
			std::cout << "Vertex " << i << "\t:("
						 << feedback[vertexStart+0] << "\t, "
						 << feedback[vertexStart+1] << "\t, "
						 << feedback[vertexStart+2] << "\t)\tlength = "
						 << feedback[vertexStart+3] << "\t normal( "
						 << feedback[vertexStart+4] << ", "
						 << feedback[vertexStart+5] << ", "
						 << feedback[vertexStart+6] << " )"
						 << std::endl;
		}
		std::cout << "-----------------------------------" << std::endl;
		*/

		shader.stopUsingProgram();

		vertexArraysAndBufers.currentVertexArray->unbind();

		//Buffer und Vertexarrays durchtauschen
		VertexArray* swapVertexArray = vertexArraysAndBufers.currentVertexArray;
		Buffer* swapTransformFeedbackBuffer = vertexArraysAndBufers.currentTransformFeedbackBuffer;
		vertexArraysAndBufers.currentVertexArray = vertexArraysAndBufers.lastVertexArray;
		vertexArraysAndBufers.currentTransformFeedbackBuffer = vertexArraysAndBufers.lastTransformFeedbackBuffer;
		vertexArraysAndBufers.lastVertexArray = swapVertexArray;
		vertexArraysAndBufers.lastTransformFeedbackBuffer = swapTransformFeedbackBuffer;
	}
}

void theGenerateButtonCallbackFunction(void *clientData) {
	ButtonCallbackParameters* params = (ButtonCallbackParameters*) clientData;

	Shaderprogram* shaderprogram = params->shader;
	shaderprogram->setUniform("scaleLength", *(params->scaleLengthUniform));
	shaderprogram->setUniform("scaleTriangle", *(params->scaleTriangleUniform));
	shaderprogram->setUniform("pyramidFactor", *(params->pyramidFactorUniform));

	generate(*(params->vertexArraysAndBufers), *(params->shader), *(params->numberOfIterations));
	*(params->numberOfVerticesToDraw) = nVertices(*(params->numberOfIterations));
	*(params->numberOfTrianglesToDraw) = nTriangles(*(params->numberOfIterations));
}

