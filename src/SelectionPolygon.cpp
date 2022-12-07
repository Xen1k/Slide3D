#include "SelectionPolygon.h"
#include "Settings.h"

void SelectionPolygon::Initialize()
{
	polygonObject = new Object(new Mesh(), new Shader("./src/shaders/unlit.shader", ShaderType::Unlit), GL_POLYGON, false);
	polygonObject->drawMulti = false;
}

void SelectionPolygon::Clear()
{
	m_Vertices.clear();
	m_Indices.clear();
	polygonObject->mesh->SetVerticesAndIndices(m_Vertices, m_Indices, false);
}

void SelectionPolygon::UpdateVertices()
{
	m_Vertices.clear();
	m_Indices.clear();
	for (int i = 0; i < polygon->vertices.size(); i++)
	{
		m_Vertices.push_back(Vertex{ polygon->GetGlobalVertexPosition(i) * 1.003f });
		m_Indices.push_back(i);
	}
	polygonObject->mesh->SetVerticesAndIndices(m_Vertices, m_Indices, false);
}

void SelectionPolygon::SetDefaultColor()
{
	SetColor(settings::polygonSelectorColor);
}

void SelectionPolygon::SetColor(glm::vec3 color)
{
	polygonObject->SetColor(color);
}

void SelectionPolygon::Render()
{
	polygonObject->Render();
}

