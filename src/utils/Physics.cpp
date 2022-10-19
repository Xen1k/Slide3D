#include "Physics.h"

bool Physics::CheckRayTriangleIntersect(Triangle triangleObj, glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3* intersectPoint)
{
	float epsilon = 0.01f; // Accuracy
	glm::vec3 e1, e2, p, s, q;
	float t, u, v, tmp; // Uvt - Barycentric Coordinates

	e1 = triangleObj.vertices[1] - triangleObj.vertices[0];
	e2 = triangleObj.vertices[2] - triangleObj.vertices[0];

	p = glm::cross(rayDir, e2);
	tmp = glm::dot(p, e1);
	if (tmp > -epsilon && tmp < epsilon) return false;

	tmp = 1.0 / tmp;
	s = rayOrigin - triangleObj.vertices[0];
	u = tmp * glm::dot(s, p);
	if (u < 0.0 || u > 1.0) return false;

	q = glm::cross(s, e1);
	v = tmp * glm::dot(rayDir, q);
	if (v < 0.0 || v > 1.0) return false;

	if (u + v > 1.0) return false;

	t = tmp * glm::dot(e2, q);

	if (intersectPoint)
		*intersectPoint = rayOrigin + t * rayDir;
	return true;
}