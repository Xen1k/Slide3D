#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"

class Camera
{
private:
	bool m_firstClick = true;
	const float m_fovDeg = 90.f;
	const float m_nearPlane = 0.1f;
	const float m_farPlane = 1000.f;
	const glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
public:
	static Camera *main;

	glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 position;

	float speed = 10.f;
	float sensitivity = 100.f;

	Camera(glm::vec3 position);

	void UpdateMatrix(Shader& shader, const char* uniform);
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	void HandleInputs(GLFWwindow* window, bool stopMouseInput = false);
};

