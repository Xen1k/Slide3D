#pragma once
#include "Mesh.h"
#include <functional>

class Object
{
private:
	glm::vec3 m_Position = glm::vec3(0.f);
	glm::mat4 m_ModelMatrix = glm::mat4(1.f);
	int m_NumOfLightSources = 0;
public:
	Object(Mesh *mesh, Shader *shader);
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
	
	static std::vector<Object*> objectsList;
	/// <summary>
	/// Calls function for each object from the list
	/// </summary>
	static void CallForEachObject(std::function<void(Object*)> callback);

	void SetPosition(float x, float y, float z);
	void Render();
	
	/// <summary>
	/// Increases number of light sources in shader
	/// </summary>
	void IncreaseNumOfLights();

	Shader* shader;
	Mesh* mesh;
};

