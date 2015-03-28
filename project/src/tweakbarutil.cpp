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

void initTweakbar(GLFWwindow* window, float windowWidth, float windowHeight) {
	TwInit(TW_OPENGL_CORE, NULL); // for core profile
	TwWindowSize(windowWidth, windowHeight);
	TwBar *bar;
	bar = TwNewBar("NameOfMyTweakBar");
	int modelRotaitonX;
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.
	TwAddVarRW(bar, "NameOfMyVariable", TW_TYPE_FLOAT, &modelRotaitonX, "RotX");


	// Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
	TwAddVarRW(bar, "speed", TW_TYPE_DOUBLE, &modelRotaitonX,
			   " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");

	// Add 'wire' to 'bar': it is a modifable variable of type TW_TYPE_BOOL32 (32 bits boolean). Its key shortcut is [w].
	TwAddVarRW(bar, "wire", TW_TYPE_BOOL32, &modelRotaitonX,
			   " label='Wireframe mode' key=w help='Toggle wireframe display mode.' ");

	// Add 'time' to 'bar': it is a read-only (RO) variable of type TW_TYPE_DOUBLE, with 1 precision digit
	TwAddVarRO(bar, "time", TW_TYPE_DOUBLE, &modelRotaitonX, " label='Time' precision=1 help='Time (in seconds).' ");

	// Add 'bgColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color)
	TwAddVarRW(bar, "bgColor", TW_TYPE_COLOR3F, &modelRotaitonX, " label='Background color' ");

	// Add 'cubeColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR32 (32 bits color) with alpha
	TwAddVarRW(bar, "cubeColor", TW_TYPE_COLOR32, &modelRotaitonX,
			   " label='Cube color' alpha help='Color and transparency of the cube.' ");

	//AntTwaekBar Callback
	// after GLFW initialization
	// directly redirect GLFW events to AntTweakBar
	glfwSetMouseButtonCallback(window,(GLFWmousebuttonfun)myMousebuttonfun);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)myCursorposfun);
	glfwSetCharCallback(window, (GLFWcharfun)myCharfun);

	// send window size events to AntTweakBar
	//glfwSetWindowSizeCallback(window,(GLFWwindowposfun) TwWindowSize); // and call TwWindowSize in the function MyResize
	//glfwSetWindowSizeCallback(window, MyResize); // and call TwWindowSize in the function MyResize
	glfwSetScrollCallback(window, (GLFWscrollfun)myScrollCallbackfun);

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
