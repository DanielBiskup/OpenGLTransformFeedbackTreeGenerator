#include "tweakbarutil.h"
#include <AntTweakBar.h>
#include <GLFW/glfw3.h>

void initTweakbar(GLFWwindow* window) {
	TwInit(TW_OPENGL_CORE, NULL); // for core profile
	TwWindowSize(windowWidth, windowHeight);
	TwBar *bar;
	bar = TwNewBar("NameOfMyTweakBar");
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
	glfwSetMouseButtonCallback(window,(GLFWmousebuttonfun)TwEventMouseButtonGLFW);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);
	//glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);
	//glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);
	//glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);

	// send window size events to AntTweakBar
	//glfwSetWindowSizeCallback(window,(GLFWwindowposfun) TwWindowSize); // and call TwWindowSize in the function MyResize
	//glfwSetWindowSizeCallback(window, MyResize); // and call TwWindowSize in the function MyResize
}
