#pragma once
#include <GL/glew.h>
#include<vector>

class EBO
{
private:
	GLuint m_ID;
public:
	EBO(std::vector<GLuint>& indices);
	~EBO();
	void Bind();
	void Unbind();
	void Delete();
};

