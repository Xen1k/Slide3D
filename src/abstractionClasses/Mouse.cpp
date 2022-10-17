#include "Mouse.h"
#include "Window.h"

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

double Mouse::GetDeviceX()
{
	return 2 * GetX() / Window::GetWidth() - 1;;
}

double Mouse::GetDeviceY()
{
	return -(2 * GetY() / Window::GetHeight() - 1);
}