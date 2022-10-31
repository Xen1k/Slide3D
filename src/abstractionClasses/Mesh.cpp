#include "Mesh.h"
#include <stdexcept>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Texture* texture, std::vector <int>& multidrawVertsCount)
{
	if (texture)
		this->texture = texture;
	SetVerticesAndIndices(vertices, indices);
	if (&multidrawVertsCount != nullptr)
	{
		this->multidrawVertsCount = multidrawVertsCount;
		GenerateMultidrawStartIndices();
	}
	SetVAO();
}

void Mesh::SetVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, bool updatePolygonsList)
{
	this->vertices = vertices;
	this->indices = indices;
	SetVAO();
	if (connectedObject && updatePolygonsList)
		connectedObject->GeneratePolygonsList();
}



void Mesh::ClearVerticesAndIndices()
{
	this->vertices = {};
	this->indices = {};
	SetVAO();
	if (connectedObject)
		connectedObject->GeneratePolygonsList();
}

void Mesh::GenerateMultidrawStartIndices()
{
	delete[] multidrawStartIndices;
	multidrawStartIndices = new void* [multidrawVertsCount.size()];
	int previousVertsCount = 0;
	multidrawStartIndices[0] = 0;
	for (int i = 0; i < multidrawVertsCount.size() - 1; i++)
	{
		multidrawStartIndices[i + 1] = reinterpret_cast<void*>((multidrawVertsCount[i] + previousVertsCount) * sizeof(unsigned int));
		previousVertsCount += multidrawVertsCount[i];
	}
}

void Mesh::SetVAO()
{
	m_VAO.Bind();
	VBO VBO(vertices);
	EBO EBO(indices);
	m_VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));


	m_VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Render(Shader& shader, Camera& camera, int drawMode, bool drawMulti)
{
	shader.Bind();
	m_VAO.Bind();

	if (texture)
	{
		shader.SetUniform1f("hasTexture", 1.f);
		texture->texUnit(shader, "tex0", 0);
		texture->Bind();
	}
	else if (glGetUniformLocation(shader.ID, "hasTexture") != -1)
	{
		shader.SetUniform1f("hasTexture", 0.f);
	}

	camera.UpdateMatrix(shader);


	if (drawMulti)
	{
		if (multidrawVertsCount.size() == 0)
			std::cout << "You need to define multidrawVertsCount for polygon multi draw mode!" << std::endl;
		else
			glMultiDrawElements(drawMode, &multidrawVertsCount[0], GL_UNSIGNED_INT, multidrawStartIndices, multidrawVertsCount.size());
	}
	else
	{
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
	}

}