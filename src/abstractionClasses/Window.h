#pragma once
#include "glfw3.h"

class Window
{
public:
	static GLFWwindow* window;
	static int GetWidth();
	static int GetHeight();
	static void ResizeViewport();
	static float GetRatio();
};

