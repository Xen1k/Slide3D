#include "camera.h"
#include "timeManager.h"
#include "Window.h"

Camera* Camera::main;

Camera::Camera(glm::vec3 position)
{
	this->position = position;
}

void Camera::UpdateMatrix(Shader& shader, const char* uniform)
{
	shader.SetUniformMat4f(uniform, GetProjectionMatrix() * GetViewMatrix());
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (m_firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			m_firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
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

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		m_firstClick = true;
	}
}