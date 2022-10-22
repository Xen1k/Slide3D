#include "Grid.h"
#include "camera.h"

Shader* Grid::m_Shader;
Mesh* Grid::m_GridMesh;

void Grid::Init()
{
	m_Shader = new Shader("./src/shaders/grid.shader");
	std::vector<Vertex> v = { Vertex{ glm::vec3(0, 0, 0) } };
	std::vector<GLuint> i = { 0, 1, 2, 3, 4, 5 };
	m_GridMesh = new Mesh(v, i);
}

void Grid::Render()
{
	m_GridMesh->Render(*m_Shader, *Camera::main);
}