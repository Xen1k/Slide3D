#include "Grid.h"
#include "camera.h"

Shader* Grid::shader;

void Grid::UpdateGrid()
{
	if (!shader)
		shader = new Shader("./src/shaders/unlit.shader");
	shader->SetUniformMat4f("view", Camera::main->GetViewMatrix());
	shader->SetUniformMat4f("proj", Camera::main->GetProjectionMatrix());
	shader->SetUniform3f("pos", glm::vec3(0.f));
}