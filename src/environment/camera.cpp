#include <iostream>
#include "camera.h"
#include "timeManager.h"
#include "Window.h"

Camera* Camera::main;

Camera::Camera(glm::vec3 position)
{
	this->position = position;
}

void Camera::UpdateMatrix(Shader& shader)
{
	shader.Bind();
	shader.SetUniformMat4f("view", GetViewMatrix());
	shader.SetUniformMat4f("projection", GetProjectionMatrix());
}


glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(m_fovDeg), Window::GetRatio(), m_nearPlane, m_farPlane);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + m_Orientation, m_Up);
}

void Camera::HandleInputs(GLFWwindow* window, bool stopMouseInput)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * m_Orientation;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * -m_Orientation;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * glm::normalize(glm::cross(m_Orientation, m_Up));
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * m_Up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * -m_Up;
	


	if (stopMouseInput)
		return;

	float width = Window::GetWidth();
	float height = Window::GetHeight();

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// Prevents camera from jumping on the first click
		if (m_firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			m_firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(86.0f))
			m_Orientation = newOrientation;

		// Rotates the Orientation left and right
		m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_firstClick = true;
	}
}