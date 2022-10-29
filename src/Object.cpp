#include <iostream>
#include "Object.h"
#include "LightSource.h"

std::vector<Object*> Object::objectsList;


Object::Object(Mesh* mesh, Shader* shader) 
{
	this->mesh = mesh;
	this->shader = shader;
	SetPosition(0.f, 0.f, 0.f);
	GenerateTrianglesList();
	this->mesh->connectedObject = this;
	objectsList.push_back(this);
}

void Object::GenerateTrianglesList()
{
	for (auto triangle : m_TrianglesList)
		delete triangle;
	m_TrianglesList.clear();
	for (int i = 0; i < mesh->indices.size(); i += 3)
	{
		m_TrianglesList.push_back(
			new Triangle(
				&mesh->vertices[mesh->indices[i]],
				&mesh->vertices[mesh->indices[i+1]],
				&mesh->vertices[mesh->indices[i+2]],
				{ mesh->indices[i], mesh->indices[i + 1], mesh->indices[i + 2] },
				&m_ModelMatrix
			)
		);
	}
}


void Object::CalculateFlatNormals()
{
	for (auto triangle : m_TrianglesList)
		triangle->SetFlatNormal();
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
	mesh->Render(_shader ? *_shader : *shader, *Camera::main);
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
