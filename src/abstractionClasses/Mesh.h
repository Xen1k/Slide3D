#pragma once
#include <iostream>
#include <string>
#include <initializer_list>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include "Converter.h"

using converter::toReference;



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
	/// <summary>
	/// Number of vertices of each polygon for polygon draw mode
	/// </summary>
	std::vector<int> multidrawVertsCount;
	/// <summary>
	/// Numbers of first vertex of each polygon for polygon draw mode. Can be generated using GenerateMultidrawStartIndices
	/// </summary>
	void** multidrawStartIndices;

	void PrintVerticesPositions();

	void GenerateMultidrawStartIndices();
	void RemovePolygon(Polygon* polygon, bool setNewVAO = true);

	Texture* texture;

	void SetVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, bool updatePolygonsList = true);
	void ClearVerticesAndIndices();


	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, 
		Texture* texture = nullptr, std::vector<int>& multidrawVertsCount = toReference<std::vector<int>>(nullptr));
	Mesh() {};

	Object* connectedObject = nullptr;
	void Render(Shader& shader, Camera& camera, int drawMode, bool drawMulti);
};

