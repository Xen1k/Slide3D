#include "Window.h"

GLFWwindow* Window::window;

int Window::GetWidth()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}

int Window::GetHeight()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return height;
}

float Window::GetRatio()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return (float)width / height;
}

void Window::ResizeViewport()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
}