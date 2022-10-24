#pragma once
#include "Object.h"
#include "Mesh.h"

class Selection;

class Selector
{
public:
	/// <summary>
	/// Highlights triangle selected with mouse raycasting
	/// </summary>
	static Triangle* SelectTriangleWithRay(glm::vec3& rayStart, glm::vec3& rayEnd);
	static Selection* lastSelection;
};

/// <summary>
/// Represents selected object and index numbers of vertices
/// </summary>
class Selection
{
public:
	Object* selectedObject;
	Triangle* selectedTriangle;
	std::vector<int> selectedVerticesIndexNumbers;
	/// <summary>
	/// Returns num of selected vertices
	/// </summary>
	inline int GetNumOfSelections() const { return selectedVerticesIndexNumbers.size(); }
};
