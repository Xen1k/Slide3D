#include "Physics.h"
#include "Mouse.h"


bool Physics::CheckRayTriangleIntersect(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3* intersectPoint)
{
	float epsilon = 0.01f; // Accuracy
	glm::vec3 e1, e2, p, s, q;
	float t, u, v, tmp; // Uvt - Barycentric Coordinates

	e1 = v1 - v0;
	e2 = v2 - v0;

	p = glm::cross(rayDir, e2);
	tmp = glm::dot(p, e1);
	if (tmp > -epsilon && tmp < epsilon) return false;

	tmp = 1.0 / tmp;
	s = rayOrigin - v0;
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

bool Physics::CheckRayPolygonIntersect(Polygon& polygon, glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3* intersectPoint)
{
	for (int i = 1; i < polygon.vertices.size() - 1; i++)
		if (CheckRayTriangleIntersect(
			polygon.GetGlobalVertexPosition(0), 
			polygon.GetGlobalVertexPosition(i), 
			polygon.GetGlobalVertexPosition(i + 1),
			rayOrigin, rayDir, intersectPoint))
			return true;
	return false;
}


glm::vec3 Physics::CastRayFromScreenPoint(double screenPointX, double screenPointY)
{
	if (screenPointX == -1)
		screenPointX = Mouse::GetDeviceX();
	if (screenPointY == -1)
		screenPointY = Mouse::GetDeviceY();
	glm::vec4 near(screenPointX, screenPointY, -1, 1);
	glm::vec4 far(screenPointX, screenPointY, 1, 1);

	glm::mat4 invMat = glm::inverse(Camera::main->GetProjectionMatrix() * Camera::main->GetViewMatrix());

	glm::vec4 nearResult = invMat * near;
	glm::vec4 farResult = invMat * far;

	nearResult /= nearResult.w;
	farResult /= farResult.w;

	return glm::normalize(glm::vec3(farResult - nearResult));
}