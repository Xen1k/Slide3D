#pragma once
#include <glfw3.h>
#include "glm/glm.hpp"

class Mouse
{
public:
	static void SetLastCoords();
	static double GetCoordsDeltaX();
	static double GetCoordsDeltaY();
	static double GetDeviceCoordsDeltaX();
	static double GetDeviceCoordsDeltaY();
	static void SetLastRayFromMousePoint();
	/// <summary>
	/// Returns vector3: last ray from mouse point - ray from mouse point
	/// </summary>
	static glm::vec3 GetRaysFromMousePointDelta();

	static glm::vec3 lastRayFromMousePoint;
	/// <summary>
	/// Get x cursor coordinate [0, windowWidth]
	/// </summary>
	static double GetX();
	static double GetY();
	static double lastDeviceX;
	static double lastDeviceY;
	static double lastX;
	static double lastY;
	/// <summary>
	/// Get x cursor coordinate [-1, 1]
	/// </summary>
	static double GetDeviceX();
	static double GetDeviceY();
};

