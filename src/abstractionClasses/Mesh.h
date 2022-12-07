#pragma once
#include <iostream>
#include <string>
#include <initializer_list>
#include <map>

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
	VBO *m_VBO;
	EBO *m_EBO;
	/// <summary>
	/// Find Indices that are already used for normals [actual index, num of usages]
	/// </summary>
	std::map<unsigned int, unsigned int> FindFlatNormalsOccupiedIndices();
	bool CheckIndicesOrderForFlatNormals();

	/// <summary>
	/// 0, 1, 2, 3 => 0
	/// </summary>
	int GetFirstPolygonVertexIndex(int polygonIndex);

	/// <summary>
	/// 0, 1, 2, 3;	 3, 4, 5, 6 => 0, 3
	/// </summary>
	vector<unsigned int> GetFirstPolygonsVerticesIndices();

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

	void PrintIndices();

	/// <summary>
	/// Rotate polygon's indices so that each polygon have it's own first vertex for flat normal
	/// </summary>
	void PrepareIndicesForFlatNormals();

	/// <summary>
	/// 0, 1, 2, 3 -> 1, 2, 3, 0
	/// </summary>
	void RotatePolygonIndices(int polygonIndex);



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

