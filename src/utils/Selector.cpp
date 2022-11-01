#include "Selector.h"
#include "Physics.h"
#include "Object.h"

Selection* Selector::lastSelection = new Selection();
SelectionMode Selector::selectionMode = SelectionMode::Face;

Polygon* Selector::SelectPolygonWithRay(glm::vec3& rayStart, glm::vec3& rayEnd)
{
	glm::vec3 intersectionPoint;
	float minRange = FLT_MAX;
	int minIndex;
	Polygon* lastSelectedPolygon = nullptr;

	for (auto obj : Object::objectsList)
	{
		minRange = FLT_MAX;
		for (auto polygon : obj->GetPolygonsList())
		{
			if (Physics::CheckRayPolygonIntersect(*polygon, rayStart, rayStart + rayEnd * 150.f, &intersectionPoint))
			{
				if (glm::length(intersectionPoint - Camera::main->position) < minRange)
				{
					minRange = glm::length(intersectionPoint - Camera::main->position);
					lastSelectedPolygon = polygon;
				}
			}
		}
		if (lastSelectedPolygon)
		{
			// Save last selection 
			lastSelection->selectedVerticesIndexNumbers.clear();
			lastSelection->selectedObject = obj;
			lastSelection->selectedPolygon = lastSelectedPolygon;
			for (int i = 0; i < lastSelectedPolygon->verticesIndexNumbers.size(); i++)
				lastSelection->selectedVerticesIndexNumbers.push_back(lastSelectedPolygon->verticesIndexNumbers[i]);
			return lastSelectedPolygon;
		}
	}
	lastSelection->selectedObject = nullptr;
	lastSelection->selectedPolygon = nullptr;
	lastSelection->selectedVerticesIndexNumbers.clear();
	return nullptr;
}


void Selection::Clear()
{
	selectedObject = nullptr;
	selectedPolygon = nullptr;
	selectedVerticesIndexNumbers.clear();;
}