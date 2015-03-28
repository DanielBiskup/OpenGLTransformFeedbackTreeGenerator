//Referenz: http://sourceforge.net/p/anttweakbar/tickets/11/

//Damit die Tasten wie Backspace richtig erkannt werden, müsste die Datei MiniGLFW.h ersetzt werden.
//Das habe ich nicht gemacht, weil ich keine Lust hatte AntTweakBar neu zu compilen, deshabl muss
//man Eingaben wenn man sie löschen oder ersetzen möchte nun vorher mit der Mouse markieren.
// http://sourceforge.net/p/anttweakbar/discussion/AntTweakBar/thread/d69d995c/#9e25/94a1

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
