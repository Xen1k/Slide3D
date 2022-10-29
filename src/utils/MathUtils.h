#pragma once
#include <glm/glm.hpp>

class Math
{
public:
	static glm::vec3 DotV3(glm::vec3 v1, glm::vec3 v2);
	static glm::vec3 DotV3(glm::vec3 v1, float v2);
};

