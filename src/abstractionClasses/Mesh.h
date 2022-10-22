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
	void PrintVector(glm::vec3 vec)
	{
		std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	}
	glm::mat4* m_ObjectModelMatrix;
public:
	Triangle(glm::vec3 *vertex1, glm::vec3 *vertex2, glm::vec3 *vertex3, std::initializer_list<unsigned int> indexNumbers, glm::mat4 *objectModel)
	{
		for (int i = 0; i < 3; i++)
			verticesIndexNumbers[i] = indexNumbers.begin()[i];
		vertices[0] = vertex1;
		vertices[1] = vertex2;
		vertices[2] = vertex3;
		m_ObjectModelMatrix = objectModel;
	}
	/// <summary>
	/// Index numbers of vertices in the mesh vertices vector
	/// </summary>
	unsigned int verticesIndexNumbers[3];
	glm::vec3* vertices[3];

	/// <summary>
	///	Returns triangle's vertices in global coordinates 
	/// </summary>
	glm::vec3 GetGlobalVertex(int vertexIndex)
	{
		return glm::vec3(*m_ObjectModelMatrix * glm::vec4(*vertices[vertexIndex], 1));
	}

	void PrintVertices() {
		std::cout << "[";
		PrintVector(*vertices[0]);
		std::cout << ", ";
		PrintVector(*vertices[1]);
		std::cout << ", ";
		PrintVector(*vertices[2]);
		std::cout << "]" << std::endl;;
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

