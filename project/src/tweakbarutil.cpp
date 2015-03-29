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

#include "tweakbarutil.h"
#include <AntTweakBar.h>
#include <GLFW/glfw3.h>

void myMousebuttonfun(GLFWwindow* window,int button,int action,int mods);
void myCursorposfun(GLFWwindow* window,double xpos,double ypos);
void myScrollCallbackfun(GLFWwindow* window, double xoffset, double yoffset);

void myKeyfun(GLFWwindow* window, int key, int scancode, int action, int modes);
void myCharfun(GLFWwindow* window, unsigned int codepoint);

void initTweakbar(GLFWwindow* window) {
	//AntTwaekBar Callback
	// after GLFW initialization
	// directly redirect GLFW events to AntTweakBar
	glfwSetMouseButtonCallback(window,(GLFWmousebuttonfun)myMousebuttonfun);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)myCursorposfun);
	glfwSetCharCallback(window, (GLFWcharfun)myCharfun);

	// send window size events to AntTweakBar
	//glfwSetWindowSizeCallback(window,(GLFWwindowposfun) TwWindowSize); // and call TwWindowSize in the function MyResize
	//glfwSetWindowSizeCallback(window, MyResize); // and call TwWindowSize in the function MyResize
	//glfwSetScrollCallback(window, (GLFWscrollfun)myScrollCallbackfun);

	//defekt:
	//glfwSetKeyCallback(window, (GLFWkeyfun)myKeyfun);
}

void myMousebuttonfun(GLFWwindow* window,int button,int action,int mods) {
	TwEventMouseButtonGLFW(button, action);
}

void myCursorposfun(GLFWwindow* window,double xpos,double ypos) {
	TwEventMousePosGLFW(xpos, ypos);
}

void myScrollCallbackfun(GLFWwindow* window, double xoffset, double yoffset) {
	TwEventMouseWheelGLFW(yoffset);
}

void myKeyfun(GLFWwindow* window, int key, int scancode, int action, int modes) {
	TwEventKeyGLFW(key,action);
}

void myCharfun(GLFWwindow* window, unsigned int codepoint) {
	TwEventCharGLFW(codepoint, GLFW_PRESS);
}
