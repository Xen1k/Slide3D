#include "MathUtils.h"

glm::vec3 Math::DotV3(glm::vec3 v1, glm::vec3 v2)
{
	return glm::vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

glm::vec3 Math::DotV3(glm::vec3 v1, float v2)
{
	return DotV3(v1, glm::vec3(v2));
}

