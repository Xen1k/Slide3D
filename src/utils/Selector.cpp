#include "Selector.h"
#include "Physics.h"
#include "Object.h"

Selection* Selector::lastSelection = new Selection();

Triangle* Selector::SelectTriangleWithRay(glm::vec3& rayStart, glm::vec3& rayEnd)
{
	glm::vec3 intersectionPoint;
	float minRange = FLT_MAX;
	int minIndex;
	Triangle* lastSelectedTriangle = nullptr;

	for (auto obj : Object::objectsList)
	{
		minRange = FLT_MAX;
		for (auto triangleObj : obj->GetTrianglesList())
		{
			if (Physics::CheckRayTriangleIntersect(*triangleObj, rayStart, rayStart + rayEnd * 150.f, &intersectionPoint))
			{
				if (glm::length(intersectionPoint - Camera::main->position) < minRange)
				{
					minRange = glm::length(intersectionPoint - Camera::main->position);
					lastSelectedTriangle = triangleObj;
				}
			}
		}
		if (lastSelectedTriangle)
		{
			// Save last selection 
			lastSelection->selectedVerticesIndexNumbers.clear();
			lastSelection->selectedObject = obj;
			lastSelection->selectedTriangle = lastSelectedTriangle;
			for (int i = 0; i < 3; i++)
				lastSelection->selectedVerticesIndexNumbers.push_back(lastSelectedTriangle->verticesIndexNumbers[i]);
			return lastSelectedTriangle;
		}
	}
	lastSelection->selectedObject = nullptr;
	lastSelection->selectedTriangle = nullptr;
	lastSelection->selectedVerticesIndexNumbers.clear();
	return nullptr;
}
