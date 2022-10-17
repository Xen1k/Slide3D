#include "LightCube.h"
#include "Object.h"

LightCube::LightCube(glm::vec3 color, int lightIndex, Shader *lightShader)
{
	this->color = color;
	m_LightShader = lightShader;
	m_LightIndex = lightIndex;

	vertices = m_CubeVertices;
	indices = m_CubeIndices;
	SetVAO();

	// Refresh shaders
	Move(0, 0, 0);
	SetColor(color);
	SetIntencity(m_Intencity);

	Object::CallForEachObject([](Object* obj) { obj->IncreaseNumOfLights(); });
}

void LightCube::Move(float x, float y, float z, bool addToPreviousPosition)
{
	lightPosition = addToPreviousPosition ? lightPosition + glm::vec3(x, y, z) : glm::vec3(x, y, z);
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), lightPosition);

	m_LightShader->Bind();
	m_LightShader->SetUniformMat4f("model", m_ModelMatrix);

	Object::CallForEachObject([=](Object* obj) {
		obj->shader->Bind();
		obj->shader->SetUniform3f("lights[" + std::to_string(m_LightIndex) + "].position", lightPosition);
		obj->shader->Unbind();
	});

}



void LightCube::SetColor(glm::vec3 color)
{
	this->color = color;

	m_LightShader->Bind();
	m_LightShader->SetUniform3f("lightColor", color);

	Object::CallForEachObject([=](Object* obj) {
		obj->shader->Bind();
		obj->shader->SetUniform3f("lights[" + std::to_string(m_LightIndex) + "].color", color);
		obj->shader->Unbind();
	});
}


void LightCube::SetIntencity(float m_Intencity)
{
	this->m_Intencity = m_Intencity;
	Object::CallForEachObject([=](Object* obj) {
		obj->shader->Bind();
		obj->shader->SetUniform1f("lights[" + std::to_string(m_LightIndex) + "].intencity", m_Intencity);
		obj->shader->SetUniform3f("lights[" + std::to_string(m_LightIndex) + "].color", color);
		obj->shader->Unbind();
	});
}

void LightCube::Render(Camera& camera) { Mesh::Render(*m_LightShader, camera); }