#include "LightSource.h"
#include <iostream>

std::vector<LightSource*> LightSource::lightsList;

LightSource::LightSource(glm::vec3 color, int lightIndex, Shader *lightShader)
{
	this->color = color;
	m_LightShader = lightShader;
	m_LightIndex = lightIndex;

	vertices = m_CubeVertices;
	indices = m_CubeIndices;
	SetVAO();

	// Refresh shaders
	Translate(0, 0, 0);
	SetColor(color);
	SetIntencity(m_Intencity);

	lightsList.push_back(this);

	Object::CallForEachObject([](Object* obj) { obj->UpdateNumOfLightSourcesInShader(); });
}

void LightSource::UpdateLightForObjectShader(Object* obj)
{
	if (obj->shader->type == ShaderType::Unlit)
		return;
	obj->UpdateNumOfLightSourcesInShader();
	obj->shader->Bind();
	for (auto light : lightsList)
	{
		obj->shader->SetUniform3f("lights[" + std::to_string(light->m_LightIndex) + "].position", light->lightPosition);
		obj->shader->SetUniform3f("lights[" + std::to_string(light->m_LightIndex) + "].color", light->color);
		obj->shader->SetUniform1f("lights[" + std::to_string(light->m_LightIndex) + "].intencity", light->m_Intencity);
	}
	obj->shader->Unbind();
}

void LightSource::Translate(float x, float y, float z, bool addToPreviousPosition)
{
	lightPosition = addToPreviousPosition ? lightPosition + glm::vec3(x, y, z) : glm::vec3(x, y, z);
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), lightPosition);

	m_LightShader->Bind();
	m_LightShader->SetUniformMat4f("model", m_ModelMatrix);



	Object::CallForEachObject([=](Object* obj) {
		if (obj->shader->type == ShaderType::Unlit)
			return;
		obj->shader->Bind();
		obj->shader->SetUniform3f("lights[" + std::to_string(m_LightIndex) + "].position", lightPosition);
		obj->shader->Unbind();
	});

}


void LightSource::SetColor(glm::vec3 color)
{
	this->color = color;

	m_LightShader->Bind();
	m_LightShader->SetUniform3f("color", color);

	Object::CallForEachObject([=](Object* obj) {
		if (obj->shader->type == ShaderType::Unlit)
			return;
		obj->shader->Bind();
		obj->shader->SetUniform3f("lights[" + std::to_string(m_LightIndex) + "].color", color);
		obj->shader->Unbind();
	});
}


void LightSource::SetIntencity(float m_Intencity)
{
	this->m_Intencity = m_Intencity;
	Object::CallForEachObject([=](Object* obj) {
		if (obj->shader->type == ShaderType::Unlit)
			return;
		obj->shader->Bind();
		obj->shader->SetUniform1f("lights[" + std::to_string(m_LightIndex) + "].intencity", m_Intencity);
		obj->shader->Unbind();
	});
}

void LightSource::Render(Camera& camera) { Mesh::Render(*m_LightShader, camera); }