#pragma once
#include <vector>
#include "VBO.h"

using std::vector;

class Polygon {
private:
	glm::mat4* m_ObjectModelMatrix;
public:
	Polygon(vector<Vertex*>& vertices, vector<unsigned int>& indexNumbers, glm::mat4* objectModel);

	/// <summary>
	/// Sets flat normal for the first triangle vertex (for flat shader normal)
	/// </summary>
	void SetFlatNormal();

	glm::vec3 Forward();

	/// <summary>
	/// Does not calculate forward vector. Gets it from flat normal.
	/// </summary>
	glm::vec3 ForwardFromNormal();

	/// <summary>
	/// Index numbers of vertices in the mesh vertices vector
	/// </summary>
	vector<unsigned int> verticesIndexNumbers;
	/// <summary>
	/// Indices of vertices index numbers in object's indices array
	/// </summary>
	vector<unsigned int> indicesOfIndexNumbers;
	/// <summary>
	/// Index of current polygon in multidrawVertsCount array of the mesh
	/// </summary>
	unsigned int indexOfMultidrawVertsCount;
	vector<Vertex*> vertices;

	/// <summary>
	///	Returns triangle's vertices in global coordinates 
	/// </summary>
	glm::vec3 GetGlobalVertexPosition(int vertexIndex);

	glm::vec3 GetVertexPosition(int vertexIndex);

};

