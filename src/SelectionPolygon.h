#pragma once
#include "Object.h"

class SelectionPolygon
{
private:
	vector<Vertex> m_Vertices;
	vector<GLuint> m_Indices;
	SelectionPolygon(SelectionPolygon const&) = delete;
	void operator=(SelectionPolygon const&) = delete;
public:
	static SelectionPolygon& GetInstance()
	{
		static SelectionPolygon instance;
		return instance;
	}

	SelectionPolygon() {};

	
	Object* polygonObject;
	Polygon* polygon;

	void Clear();
	void Initialize();
	void UpdateVertices();
	void SetDefaultColor();
	void SetColor(glm::vec3 color);
	void Render();

};

