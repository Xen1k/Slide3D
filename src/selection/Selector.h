#pragma once
#include "Object.h"
#include "Mesh.h"

class Selection;

enum class SelectionMode { Face, Object };

class Selector
{
public:
	/// <summary>
	/// Highlights triangle selected with mouse raycasting
	/// </summary>
	static Polygon* SelectPolygonWithRay(glm::vec3& rayStart, glm::vec3& rayEnd);
	static Selection* lastSelection;
	static SelectionMode selectionMode;
};


/// <summary>
/// Represents selected object and index numbers of vertices
/// </summary>
class Selection
{
public:
	void Clear();
	Object* selectedObject;
	Polygon* selectedPolygon;
	std::vector<unsigned int> selectedVerticesIndexNumbers;
	void SelectPolygon(Polygon* polygon, Object* obj);
	/// <summary>
	/// Returns num of selected vertices
	/// </summary>
	inline int GetNumOfSelections() const { return selectedVerticesIndexNumbers.size(); }
};

