#pragma once
#include <functional>
#include "Polygon.h"

class Object;
#include "Mesh.h"

class Object
{
private:
	glm::vec3 m_Position = glm::vec3(0.f);
	glm::mat4 m_ModelMatrix = glm::mat4(1.f);
	std::vector<Polygon*> m_PolygonsList;
public:
	Object(Mesh *mesh, Shader *shader, int drawMode = GL_POLYGON);
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
	inline std::vector<Polygon*> GetPolygonsList() const { return m_PolygonsList; }

	int drawMode;

	/// <summary>
	/// Specifies what to call: glMultiDrawElements or glDrawElements
	/// </summary>
	bool drawMulti = true;

	void CalculateFlatNormals();

	/// <summary>
	/// List with all objects
	/// </summary>
	static std::vector<Object*> objectsList;
	/// <summary>
	/// Calls function for each object from the list
	/// </summary>
	static void CallForEachObject(std::function<void(Object*)> callback);

	/// <summary>
	/// Sets all polygons coordinates in world space to polygonsList
	/// </summary>
	void GeneratePolygonsList();

	void SetShader(Shader* shader);
	
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 vec);
	void Translate(float x, float y, float z);
	void Translate(glm::vec3 v);

	void SetColor(glm::vec3 color);

	void Render(Shader* _shader = nullptr);
	
	/// <summary>
	/// Increases number of light sources in shader
	/// </summary>
	void UpdateNumOfLightSourcesInShader();

	Shader* shader;
	Mesh* mesh;
};

