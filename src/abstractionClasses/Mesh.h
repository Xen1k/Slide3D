#pragma once
#include <iostream>
#include <string>
#include <initializer_list>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"



class Triangle {
private:
	glm::mat4* m_ObjectModelMatrix;
public:
	Triangle(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3, std::initializer_list<unsigned int> indexNumbers, glm::mat4 *objectModel)
	{
		for (int i = 0; i < 3; i++)
			verticesIndexNumbers[i] = indexNumbers.begin()[i];
		vertices[0] = vertex1;
		vertices[1] = vertex2;
		vertices[2] = vertex3;
		m_ObjectModelMatrix = objectModel;
	}

	/// <summary>
	/// Sets flat normal for the first triangle vertex (for flat shader normal)
	/// </summary>
	void SetFlatNormal()
	{
		vertices[0]->normal = Forward();
	}

	glm::vec3 Forward()
	{
		return glm::normalize(glm::cross(vertices[1]->position - vertices[0]->position, vertices[2]->position - vertices[0]->position));
	}

	/// <summary>
	/// Does not calculate forward vector. Gets it from flat normal.
	/// </summary>
	glm::vec3 ForwardFromNormal()
	{
		return vertices[0]->normal;
	}

	/// <summary>
	/// Index numbers of vertices in the mesh vertices vector
	/// </summary>
	unsigned int verticesIndexNumbers[3];
	Vertex* vertices[3];

	/// <summary>
	///	Returns triangle's vertices in global coordinates 
	/// </summary>
	glm::vec3 GetGlobalVertexPosition(int vertexIndex)
	{
		return glm::vec3(*m_ObjectModelMatrix * glm::vec4(vertices[vertexIndex]->position, 1));
	}

	glm::vec3 GetVertexPosition(int vertexIndex)
	{
		return vertices[vertexIndex]->position;
	}

};

class Mesh;

#include "Object.h";

class Mesh
{
private:
	VAO m_VAO;
public:
	void SetVAO();
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Texture* texture;

	void SetVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, bool updateTrianglesList = true);
	void ClearVerticesAndIndices();


	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, Texture* texture = nullptr);
	Mesh() {};

	Object* connectedObject = nullptr;
	void Render(Shader& shader, Camera& camera);
};

