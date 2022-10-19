#pragma once
#include "Mesh.h"
#include <functional>

class Object
{
private:
	glm::vec3 m_Position = glm::vec3(0.f);
	glm::mat4 m_ModelMatrix = glm::mat4(1.f);
	std::vector<Triangle*> m_TrianglesList;
public:
	Object(Mesh *mesh, Shader *shader);
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
	inline std::vector<Triangle*> GetTrianglesList() const { return m_TrianglesList; }

	/// <summary>
	/// List with all objects
	/// </summary>
	static std::vector<Object*> objectsList;
	/// <summary>
	/// Calls function for each object from the list
	/// </summary>
	static void CallForEachObject(std::function<void(Object*)> callback);

	/// <summary>
	/// Sets all triangles coordinates in world space to trianglesList
	/// </summary>
	void UpdateTrianglesList();

	void SetShader(Shader* shader);
	
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 vec);
	void Translate(float x, float y, float z);

	void Render();
	
	/// <summary>
	/// Increases number of light sources in shader
	/// </summary>
	void UpdateNumOfLightSourcesInShader();

	Shader* shader;
	Mesh* mesh;
};

