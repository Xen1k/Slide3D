#include "Polygon.h"
#include <iostream>

Polygon::Polygon(vector<Vertex*>& vertices, vector<unsigned int>& indexNumbers, glm::mat4* objectModel)
{
	verticesIndexNumbers = indexNumbers;
	this->vertices = vertices;
	m_ObjectModelMatrix = objectModel;
}

void Polygon::SetFlatNormal() 
{ 
	vertices[0]->normal = Forward();
}

glm::vec3 Polygon::Forward()
{
	return glm::normalize(glm::cross(vertices[1]->position - vertices[0]->position, vertices[2]->position - vertices[0]->position));
}

glm::vec3 Polygon::ForwardFromNormal() { return vertices[0]->normal; }

glm::vec3 Polygon::GetGlobalVertexPosition(int vertexIndex)
{
	return glm::vec3(*m_ObjectModelMatrix * glm::vec4(vertices[vertexIndex]->position, 1));
}

glm::vec3 Polygon::GetVertexPosition(int vertexIndex)
{
	return vertices[vertexIndex]->position;
}