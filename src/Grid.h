#pragma once
#include "Object.h"

class Grid
{
private:
	static Shader* m_Shader;
	static Mesh* m_GridMesh;
public:
	static void Init();
	static void Render();
};

