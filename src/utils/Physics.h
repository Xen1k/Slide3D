#pragma once
#include "Mesh.h"

class Physics
{
public:
	static bool CheckRayTriangleIntersect(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3* intersectPoint = nullptr);
	static bool CheckRayPolygonIntersect(Polygon& polygon, glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3* intersectPoint);
	static glm::vec3 CastRayFromScreenPoint(double screenPointX = -1, double screenPointY = -1);
};

