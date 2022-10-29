#include "Physics.h"
#include "Mouse.h"
#include "Window.h"

double Mouse::lastDeviceX;
double Mouse::lastDeviceY;
double Mouse::lastX;
double Mouse::lastY;
glm::vec3 Mouse::lastRayFromMousePoint;

double Mouse::GetX()
{
	double xPos, yPos;
	glfwGetCursorPos(Window::window, &xPos, &yPos);
	return xPos;
}

double Mouse::GetY()
{
	double xPos, yPos;
	glfwGetCursorPos(Window::window, &xPos, &yPos);
	return yPos;
}

glm::vec3 Mouse::GetRaysFromMousePointDelta()
{
	return Physics::CastRayFromScreenPoint() - lastRayFromMousePoint;
}

void Mouse::SetLastRayFromMousePoint()
{
	lastRayFromMousePoint = Physics::CastRayFromScreenPoint();
}

void Mouse::SetLastCoords()
{
	lastDeviceX = GetDeviceX();
	lastDeviceY = GetDeviceY();
	lastX = GetX();
	lastY = GetY();
}

double Mouse::GetCoordsDeltaX()
{
	return lastX - GetX();
}

double Mouse::GetCoordsDeltaY()
{
	return lastY - GetY();
}

double Mouse::GetDeviceCoordsDeltaX()
{
	return lastDeviceX - GetDeviceX();
}

double Mouse::GetDeviceCoordsDeltaY()
{
	return lastDeviceY - GetDeviceY();
}

double Mouse::GetDeviceX()
{
	return 2 * GetX() / Window::GetWidth() - 1;;
}

double Mouse::GetDeviceY()
{
	return -(2 * GetY() / Window::GetHeight() - 1);
}