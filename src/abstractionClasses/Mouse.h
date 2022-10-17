#pragma once
#include <glfw3.h>

class Mouse
{
public:
	/// <summary>
	/// Get x cursor coordinate [0, windowWidth]
	/// </summary>
	static double GetX();
	static double GetY();
	/// <summary>
	/// Get x cursor coordinate [-1, 1]
	/// </summary>
	static double GetDeviceX();
	static double GetDeviceY();
};

