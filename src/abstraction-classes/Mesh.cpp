#include "Mesh.h"
#include <stdexcept>
#include <algorithm>

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

void Mesh::RemovePolygon(Polygon* polygon, bool setNewVAO)
{
	if (connectedObject->GetPolygonsList().size() == 1)
		return;
	for (int i = 0; i < polygon->indicesOfIndexNumbers.size(); i++)
	{
		// Indices should be in the right order!
		if(connectedObject->GetPolygonsList()[connectedObject->GetPolygonsList().size() - 1] != polygon)
			indices[polygon->indicesOfIndexNumbers[i]] = indices.back();
		indices.pop_back();
	}
	multidrawVertsCount[polygon->indexOfMultidrawVertsCount] = multidrawVertsCount.back();
	multidrawVertsCount.pop_back();
	if (setNewVAO)
	{
		if (connectedObject)
		{
			connectedObject->GeneratePolygonsList();
			connectedObject->CalculateFlatNormals();
		}
		GenerateMultidrawStartIndices();
		SetVAO();
	}
}


void Mesh::SetVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, bool updatePolygonsList)
{
	this->vertices = vertices;
	this->indices = indices;
	SetVAO();

	if (!connectedObject)
		return;
	connectedObject->CalculateFlatNormals();
	if (updatePolygonsList)
		connectedObject->GeneratePolygonsList();
}


void Mesh::PrintVerticesPositions()
{
	std::cout << "Vertices positions list:" << std::endl;
	for (auto v : vertices)
		std::cout << "(" << v.position.x << ", " << v.position.y << ", " << v.position.z << ")\n";
}

void Mesh::ClearVerticesAndIndices()
{
	this->vertices = {};
	this->indices = {};
	SetVAO();
	if (connectedObject)
		connectedObject->GeneratePolygonsList();
}

void Mesh::PrintIndices()
{
	int overallVertsCount = 0;
	for (int i = 0; i < multidrawVertsCount.size(); i++)
	{
		for (int j = overallVertsCount; j < overallVertsCount + multidrawVertsCount[i]; j++)
			std::cout << indices[j] << " ";
		overallVertsCount += multidrawVertsCount[i];
		std::cout << std::endl;
	}
	std::cout << std::endl;
}



bool Mesh::CheckIndicesOrderForFlatNormals()
{
	std::map<unsigned int, unsigned int> occupiedIndices = FindFlatNormalsOccupiedIndices();
	for (auto it = occupiedIndices.begin(); it != occupiedIndices.end(); it++)
	{
		if (it->second > 1)
			return false;
	}
	return true;
}

std::map<unsigned int, unsigned int> Mesh::FindFlatNormalsOccupiedIndices()
{
	int overallVertsCount = 0;
	std::map<unsigned int, unsigned int> occupiedIndices;
	for (int i = 0; i < multidrawVertsCount.size(); i++) {
		occupiedIndices[indices[overallVertsCount]]++;
		overallVertsCount += multidrawVertsCount[i];
	}
	return occupiedIndices;
}

void Mesh::RotatePolygonIndices(int polygonIndex)
{
	// polygon index: 
	// 0: 0, 1, 2, 3
	// 1: 1, 2, 3, 0
	// ...
	int numOfVertsBeforePolygon = 0;
	for (int i = 0; i < multidrawVertsCount.size(); i++)
	{
		if (i == polygonIndex)
			break;
		numOfVertsBeforePolygon += multidrawVertsCount[i];
	}
	int temp = indices[numOfVertsBeforePolygon];
	for (int i = 0; i < multidrawVertsCount[polygonIndex] - 1; i++)
		indices[numOfVertsBeforePolygon + i] = indices[numOfVertsBeforePolygon + i + 1];
	indices[numOfVertsBeforePolygon + multidrawVertsCount[polygonIndex] - 1] = temp;
}

int Mesh::GetFirstPolygonVertexIndex(int polygonIndex)
{
	int numOfVertsBeforePolygon = 0;
	for (int i = 0; i < multidrawVertsCount.size(); i++)
	{
		if (i == polygonIndex)
			return indices[numOfVertsBeforePolygon];
		numOfVertsBeforePolygon += multidrawVertsCount[i];
	}
	return -1;
}

vector<unsigned int> Mesh::GetFirstPolygonsVerticesIndices()
{
	vector<unsigned int> firstIndices;
	int numOfVertsBeforePolygon = 0;
	for (int i = 0; i < multidrawVertsCount.size(); i++)
	{
		firstIndices.push_back(indices[numOfVertsBeforePolygon]);
		numOfVertsBeforePolygon += multidrawVertsCount[i];
	}
	return firstIndices;
}



void Mesh::PrepareIndicesForFlatNormals()
{
	std::map<unsigned int, unsigned int> occupiedIndices = FindFlatNormalsOccupiedIndices();
	vector<unsigned int> indNumsOfRotatedIndices;
	int overallVertsCount = 0;
	bool indicesWereEdited;
	auto it = occupiedIndices.begin();
	for (;;)
	{
		indicesWereEdited = false;
		overallVertsCount = 0;
		if (it->second > 1)
		{
			for (int i = 0; i < multidrawVertsCount.size(); i++)
			{
				if (indices[overallVertsCount] == it->first)
				{
					if (std::find(indNumsOfRotatedIndices.begin(), indNumsOfRotatedIndices.end(), overallVertsCount) != indNumsOfRotatedIndices.end())
						continue;

					auto firstPolygonsIndices = GetFirstPolygonsVerticesIndices();
					bool foundAnotherSameFirstIndex = false;

					for (int j = 0; j < multidrawVertsCount[i]; j++)
					{
						foundAnotherSameFirstIndex = false;
						RotatePolygonIndices(i);
						for (int k = 0; k < multidrawVertsCount.size(); k++)
						{
							if (k != i && GetFirstPolygonVertexIndex(i) == GetFirstPolygonsVerticesIndices()[k])
							{
								foundAnotherSameFirstIndex = true;
								break;
							}
						}

						if (!foundAnotherSameFirstIndex)
						{
							std::cout << "!!!!!!!!!!!!!!!!!";
							indNumsOfRotatedIndices.push_back(overallVertsCount);
							break;
						}
					}
					// Find occupied indices again
					occupiedIndices = FindFlatNormalsOccupiedIndices();
					it = occupiedIndices.begin();
					indicesWereEdited = true;
					PrintIndices();
					break;
				}
				overallVertsCount += multidrawVertsCount[i];
			}
		}
		if (!indicesWereEdited)
			it++;
		if (it == occupiedIndices.end())
		{
			//if(CheckIndicesOrderForFlatNormals())
				break;
			/*occupiedIndices = FindFlatNormalsOccupiedIndices();
			it = occupiedIndices.begin();*/
			//indNumsOfRotatedIndices.clear();
		}
	 }


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
	if (m_VBO)
	{
		delete m_VBO;
		delete m_EBO;
	}
	m_VBO = new VBO(vertices);
	m_EBO = new EBO(indices);
	m_VAO.LinkAttrib(*m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_VAO.LinkAttrib(*m_VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_VAO.LinkAttrib(*m_VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));


	m_VAO.Unbind();
	m_VBO->Unbind();
	m_EBO->Unbind();
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