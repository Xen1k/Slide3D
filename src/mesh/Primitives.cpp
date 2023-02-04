#include "Primitives.h"

void Primitives::SetSphereVertices(vector<Vertex>*vertices, vector<GLuint>*indices, float radius, unsigned int rings, unsigned int sectors)
{
	std::vector<float> vertices_;
	std::vector<float> normals;
	std::vector<float> texCoords;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / rings;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= rings; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        
		xy = radius * cosf(stackAngle);             
		z = radius * sinf(stackAngle);              

		for (int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;           

		
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices_.push_back(x);
			vertices_.push_back(y);
			vertices_.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectors;
			t = (float)i / rings;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	int j = 0;
	vertices->clear();
	for (int i = 0; i < vertices_.size(); i += 3, j += 2)
		vertices->push_back(Vertex{
			glm::vec3(vertices_[i], vertices_[i + 1], vertices_[i + 2]),
			glm::vec2(texCoords[j], texCoords[j + 1]),
			glm::vec3(normals[i], normals[i + 1], normals[i + 2])
			});

	indices->clear();
	int k1, k2;
	for (int i = 0; i < rings; ++i)
	{
		k1 = i * (sectors + 1);     
		k2 = k1 + sectors + 1;      

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices->push_back(k1);
				indices->push_back(k2);
				indices->push_back(k1 + 1);
			}

			if (i != (rings - 1))
			{
				indices->push_back(k1 + 1);
				indices->push_back(k2);
				indices->push_back(k2 + 1);
			}

		}
	}
}

void Primitives::SetPyramidVertices(vector<Vertex>*vertices, vector<GLuint>*indices)
{
	*vertices =
	{
		// Bottom
		Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(0.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(5.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},

		// Left
		Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)},
		Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(-0.8f, 0.5f, 0.0f)},

		// Back
		Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(5.0f, 0.f), glm::vec3(0.0f, 0.5f, -0.8f)},
		Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f, -0.8f)},

		// Front
		Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)},
		Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.8f, 0.5f,  0.0f)},

		// Front
		Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
		Vertex{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
	};

	*indices =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		7, 9, 8,
		10, 12, 11,
		13, 15, 14
	};

}

void Primitives::SetCubeVertices(vector<Vertex>* vertices, vector<GLuint>* indices, vector<int>* multidrawVertsCount)
{

	*vertices = {
		Vertex{glm::vec3(-1, -1	, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)},
		Vertex{glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)},
		Vertex{glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, 0)},
		Vertex{glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(1, 0, 0)},
		Vertex{glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)},
		Vertex{glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 0)},
		Vertex{glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)},
		Vertex{glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)},
	};

	*indices = {
		0, 2, 3, 1,
		//4, 6, 7, 5,
		7, 5, 4, 6,
		2, 4, 5, 3,
		7, 6, 0, 1,
		6, 4, 2, 0,
		1, 3, 5, 7,
	};

	*multidrawVertsCount = { 4, 4, 4, 4, 4, 4 };

}

void Primitives::SetCubeVertices(vector<Vertex>* vertices, vector<GLuint>* indices)
{

	*vertices = {
		Vertex{glm::vec3(-1, -1	, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)},
		Vertex{glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)},
		Vertex{glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, 0)},
		Vertex{glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(1, 0, 0)},
		Vertex{glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)},
		Vertex{glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 0)},
		Vertex{glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)},
		Vertex{glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)},
	};

	*indices = { 
		0, 2, 3, 0, 3, 1, // front
		4, 6, 7, 4, 7, 5, // back
		3, 2, 4, 3, 4, 5, // right
		7, 6, 0, 7, 0, 1, // left
		6, 4, 2, 6, 2, 0, // bottom 
		1, 3, 5, 1, 5, 7  // top
	};
}