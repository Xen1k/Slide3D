#pragma once
#include "Mesh.h"

class Physics
{
public:
	static bool CheckRayTriangleIntersect(Triangle triangleObj, glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3* intersectPoint = nullptr);
	static glm::vec3 CastRayFromScreenPoint();

};

