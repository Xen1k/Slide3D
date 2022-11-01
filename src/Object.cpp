#include <iostream>
#include "Object.h"
#include "LightSource.h"

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
