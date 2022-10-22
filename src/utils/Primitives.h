#pragma once
#include <vector>
#include "VBO.h"

using std::vector;


constexpr float PI = 3.1415926535979f;

class Primitives
{
public:
	static void SetPyramidVertices(vector<Vertex>* vertices, vector<GLuint>* indices);
	static void SetSphereVertices(vector<Vertex>* vertices, vector<GLuint>* indices, float radius, unsigned int rings, unsigned int sectors);
	static void SetCubeVertices(vector<Vertex>* vertices, vector<GLuint>* indices);
};