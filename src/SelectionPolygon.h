#pragma once
#include "Object.h"

class SelectionPolygon
{
private:
	vector<Vertex> m_Vertices;
	vector<GLuint> m_Indices;
public:
	Object* polygonObject;
	Polygon* polygon;

	SelectionPolygon();

	void UpdateVertices();
	void SetDefaultColor();
	void SetColor(glm::vec3 color);
	void Render();

};

