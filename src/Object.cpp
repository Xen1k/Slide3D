#include <iostream>
#include "Object.h"
#include "LightSource.h"
#include "SelectionPolygon.h"
#include "Selector.h"
#include "MathUtils.h"

std::vector<Object*> Object::objectsList;


Object::Object(Mesh* mesh, Shader* shader, int drawMode, bool addToObjectsList) : drawMode(drawMode)
{
	this->mesh = mesh;
	this->shader = shader;
	SetPosition(0.f, 0.f, 0.f);
	GeneratePolygonsList();
	this->mesh->connectedObject = this;
	CalculateFlatNormals();
	if(addToObjectsList)
		objectsList.push_back(this);
}

void Object::RemoveSelectedPolygon()
{
	auto selectedPolygon = Selector::lastSelection->selectedPolygon;
	if (!selectedPolygon)
		return;
	Selector::lastSelection->Clear();
	SelectionPolygon::GetInstance().Clear();
	mesh->RemovePolygon(selectedPolygon);
}

void Object::ExtrudeSelection()
{
	auto selectedPolygon = Selector::lastSelection->selectedPolygon;
	if (!selectedPolygon)
		return;

	SelectionPolygon::GetInstance().Clear();
	// Remove old polygon
	mesh->RemovePolygon(selectedPolygon, false);

	vector<unsigned int> newIndices;
	// Indices
	for (int i = 0; i < selectedPolygon->verticesIndexNumbers.size(); i++)
	{
		mesh->indices.push_back(mesh->vertices.size() + i);
		newIndices.push_back(mesh->vertices.size() + i);
	}
	for (int i = 0; i < selectedPolygon->vertices.size() - 1; i++)
	{
		mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[i]);
		mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[i + 1]);
		mesh->indices.push_back(newIndices[i + 1]);
		mesh->indices.push_back(newIndices[i]);
	}

	mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[selectedPolygon->vertices.size() - 1]);
	mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[0]);
	mesh->indices.push_back(newIndices[0]);
	mesh->indices.push_back(newIndices[selectedPolygon->vertices.size() - 1]);


	// Vertices
	vector<Vertex> newVertices;
	for (auto vertex : selectedPolygon->vertices)
	{
		newVertices.push_back(*vertex);
		newVertices[newVertices.size() - 1].position += Math::DotV3(selectedPolygon->Forward(), 1.5f);
	}


	mesh->vertices.insert(mesh->vertices.end(), newVertices.begin(), newVertices.end());

	for (int i = 0; i < selectedPolygon->vertices.size() + 1 /* Sides + new front face */; i++)
		mesh->multidrawVertsCount.push_back(selectedPolygon->verticesIndexNumbers.size());



	mesh->GenerateMultidrawStartIndices();
	GeneratePolygonsList();
	SelectionPolygon::GetInstance().polygon = GetPolygonsList()[GetPolygonsList().size() - 5];
	Selector::lastSelection->SelectPolygon(SelectionPolygon::GetInstance().polygon, this);
	mesh->SetVAO();
}


void Object::GeneratePolygonsList()
{
	for (auto polygon : m_PolygonsList)
		delete polygon;
	m_PolygonsList.clear();

	int previousVertsCount = 0;
	vector<Vertex*> polygonVertices;
	vector<unsigned int> polygonIndices;
	vector<unsigned int> polygonIndicesOfIndexNumbers; // Look indicesOfIndexNumbers in Polygon class
	for (int i = 0; i < mesh->multidrawVertsCount.size(); i++)
	{
		polygonVertices.clear();
		polygonIndices.clear();
		polygonIndicesOfIndexNumbers.clear();
		for (int j = 0; j < mesh->multidrawVertsCount[i]; j++)
		{
			polygonVertices.push_back(&mesh->vertices[mesh->indices[previousVertsCount + j]]);
			polygonIndices.push_back(mesh->indices[previousVertsCount + j]);
			polygonIndicesOfIndexNumbers.push_back(previousVertsCount + j);
		}
		m_PolygonsList.push_back(new Polygon(polygonVertices, polygonIndices, &m_ModelMatrix));
		// Set indicesOfIndexNumbers
		m_PolygonsList[m_PolygonsList.size() - 1]->indicesOfIndexNumbers = polygonIndicesOfIndexNumbers;
		m_PolygonsList[m_PolygonsList.size() - 1]->indexOfMultidrawVertsCount = i;
		previousVertsCount += mesh->multidrawVertsCount[i];
	}
}


void Object::CalculateFlatNormals()
{
	for (auto polygon : m_PolygonsList)
		polygon->SetFlatNormal();
	mesh->SetVAO();
}

void Object::SetShader(Shader* shader)
{
	delete this->shader;
	this->shader = shader;
	shader->Bind();
	shader->SetUniformMat4f("model", m_ModelMatrix);
	if (shader->type != ShaderType::Unlit)
		LightSource::UpdateLightForObjectShader(this);
	shader->Unbind();
}

void Object::SetPosition(float x, float y, float z)
{
	m_Position = glm::vec3(x, y, z);
	m_ModelMatrix = glm::translate(glm::mat4(1), m_Position);
	shader->Bind();
	shader->SetUniformMat4f("model", m_ModelMatrix);
}

void Object::SetColor(glm::vec3 color)
{
	shader->Bind();
	shader->SetUniform3f("color", color);
	shader->Unbind();
}

void Object::SetPosition(glm::vec3 vec)
{
	SetPosition(vec.x, vec.y, vec.z);
}

void Object::Translate(float x, float y, float z)
{
	SetPosition(m_Position.x +  x, m_Position.y + y, m_Position.z + z);
}

void Object::Translate(glm::vec3 v)
{
	Translate(v.x, v.y, v.z);
}



void Object::Render(Shader *_shader)
{
	if (_shader)
	{
		_shader->Bind();
		_shader->SetUniformMat4f("model", m_ModelMatrix);
	}
	mesh->Render(_shader ? *_shader : *shader, *Camera::main, drawMode, drawMulti);
}

void Object::UpdateNumOfLightSourcesInShader()
{
	if (shader->type == ShaderType::Unlit)
		return;
	shader->Bind();
	shader->SetUniform1i("numOfLights", LightSource::lightsList.size());
	shader->Unbind();
}

void Object::CallForEachObject(std::function<void(Object*)> callback)
{
	for (auto obj : objectsList)
		callback(obj);
}
