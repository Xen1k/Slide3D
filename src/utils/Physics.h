#pragma once
#include "Mesh.h"

class Physics
{
public:
	static bool CheckRayTriangleIntersect(Triangle selectionTriangleObj, glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3* intersectPoint = nullptr);
	static glm::vec3 CastRayFromScreenPoint(double screenPointX = -1, double screenPointY = -1);

};

