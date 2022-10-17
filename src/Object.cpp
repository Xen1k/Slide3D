#include "Object.h"

std::vector<Object*> Object::objectsList;


Object::Object(Mesh* mesh, Shader* shader) 
{
	this->mesh = mesh;
	this->shader = shader;
	SetPosition(0.f, 0.f, 0.f);
	objectsList.push_back(this);
}

void Object::SetPosition(float x, float y, float z)
{
	m_Position = glm::vec3(x, y, z);
	m_ModelMatrix = glm::translate(glm::mat4(1), m_Position);
	shader->Bind();
	shader->SetUniformMat4f("model", m_ModelMatrix);
}

void Object::Render() 
{
	mesh->Render(*shader, *Camera::main);
}

void Object::IncreaseNumOfLights()
{
	shader->Bind();
	m_NumOfLightSources++;
	shader->SetUniform1i("numOfLights", m_NumOfLightSources);
	shader->Unbind();
}

void Object::CallForEachObject(std::function<void(Object*)> callback)
{
	for (auto obj : objectsList)
		callback(obj);
}
