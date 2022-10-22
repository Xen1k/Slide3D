#pragma comment(lib, "GlU32.Lib")

#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>
#include <stb/stb_image.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "camera.h"
#include "Mesh.h"
#include "LightSource.h"
#include "Object.h"
#include "Mouse.h"
#include "Window.h"
#include "Physics.h"
#include "Selector.h"
#include "Grid.h"
#include "Primitives.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "timeManager.h"

const unsigned int width = 860;
const unsigned int height = 660;

using std::cout;
using std::endl;
using std::vector;

vector<Vertex> vertices;

vector<GLuint> indices;

void InitializeDependenciesAndWindow(GLFWwindow** window);
void PrintVector(glm::vec3 vec) { cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << endl; }


Object* triangleObj;
Triangle* selectionTriangle;
Shader* linesShader;

void SetSelectionTriangleVertices(Triangle triangle)
{
	vertices.clear();
	for(int i = 0; i < 3; i++)
		vertices.push_back(Vertex{ triangle.GetGlobalVertex(i) * 1.0001f });
	indices = { 0, 1, 2 };
	triangleObj->mesh->SetVerticesAndIndices(vertices, indices, false);
}


int main()
{
	GLFWwindow* window;
	InitializeDependenciesAndWindow(&window);
	Camera::main = new Camera(glm::vec3(0.0f, 0.0f, 1.0f));
	Grid::Init();

	triangleObj = new Object(new Mesh(), new Shader("./src/shaders/unlit.shader", ShaderType::Unlit));

	Texture* texture = new Texture("./textures/pixel.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	Primitives::SetSphereVertices(&vertices, &indices, 0.5f, 9, 9);
	Object *object = new Object(new Mesh(vertices, indices), new Shader("./src/shaders/lit.shader"));
	object->shader->SetUniform3f("color", glm::vec3(1.f, 1.f, 1.f));

	//linesShader = new Shader("./src/shaders/unlit.shader", ShaderType::Unlit);
	//linesShader->SetUniform3f("color", glm::vec3(1.f, 0.f, 1.f));

	LightSource light(glm::vec3(1.f, 1.f, 1.0f), 0, new Shader("./src/shaders/unlit.shader", ShaderType::Unlit));

	bool mouseIsOverMeshGui = false;


	bool lightIsEnabled = true;

	bool showWireframe = true;
	bool showGrid = true;

	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1, -1);


	while (!glfwWindowShouldClose(window))
	{
		Time::SetNow();
		Window::ResizeViewport();
		glClearColor(60.f / 255, 60.f / 255, 60.f / 255, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLineWidth(1.5f / glm::distance(object->GetPosition(), Camera::main->position));

	
		if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		{
			Object* obj = Selector::lastSelection->selectedObject;
			if (obj)
			{
				for (int i = 0; i < 3; i++)
				{
					obj->mesh->vertices[Selector::lastSelection->selectedVerticesIndexNumbers[i]].position.x += 2 * Time::GetDeltaTime();
				}
				SetSelectionTriangleVertices(*selectionTriangle);
				obj->mesh->SetVAO();
			}
		}

		Camera::main->HandleInputs(window, mouseIsOverMeshGui);
		
		if (selectionTriangle)
			SetSelectionTriangleVertices(*selectionTriangle);
		else
			triangleObj->mesh->ClearVerticesAndIndices();

		if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
		{
			glm::vec3 lastRayStart = glm::vec3(Camera::main->position.x, Camera::main->position.y, Camera::main->position.z);
			glm::vec3 lastRayEnd = Physics::CastRayFromScreenPoint();
			selectionTriangle = Selector::SelectTriangleWithRay(lastRayStart, lastRayEnd);
		}


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (auto obj : Object::objectsList)
		{
			if (obj != triangleObj)
			{
				obj->shader->Bind();
				obj->shader->SetUniform3f("color", glm::vec3(0.8f));
			}
			obj->Render();
		}


		if (showWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			for (auto obj : Object::objectsList)
			{
				if (obj != triangleObj)
				{
					obj->shader->Bind();
					obj->shader->SetUniform3f("color", glm::vec3(0.f));
					obj->Render(linesShader);
				}
			}
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if(lightIsEnabled) light.Render(*Camera::main);

		if(showGrid) Grid::Render();


#pragma region GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Mesh/Texture/Light");
		mouseIsOverMeshGui = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
		ImGui::Text("Mesh");
		if (ImGui::Button("Sphere"))
		{
			Primitives::SetSphereVertices(&vertices, &indices, 0.5f, 10, 10);
			selectionTriangle = nullptr;
			object->mesh->SetVerticesAndIndices(vertices, indices);
		}
		if (ImGui::Button("Pyramid"))
		{
			Primitives::SetPyramidVertices(&vertices, &indices);
			selectionTriangle = nullptr;
			object->mesh->SetVerticesAndIndices(vertices, indices);
		}
		if (ImGui::Button("Cube"))
		{
			Primitives::SetCubeVertices(&vertices, &indices);
			selectionTriangle = nullptr;
			object->mesh->SetVerticesAndIndices(vertices, indices);
		}
		ImGui::Text("View");
		if (ImGui::Button("Switch Texture"))
		{
			Object::CallForEachObject([&texture](Object* object) { 
				object->mesh->texture = object->mesh->texture ? nullptr : texture;
			});
		}
		if (ImGui::Button("Switch Shader"))
		{
			for (auto obj : Object::objectsList)
			{
				if (obj->shader->type == ShaderType::Lit)
					obj->SetShader(new Shader("./src/shaders/unlit.shader", ShaderType::Unlit));
				else
					obj->SetShader(new Shader("./src/shaders/lit.shader", ShaderType::Lit));
				obj->shader->Bind();
				obj->shader->SetUniform3f("color", glm::vec3(0.8f));
			}
		}
		if (ImGui::Button("Switch Wireframe"))
			showWireframe = !showWireframe;
		if (ImGui::Button("Switch Grid"))
			showGrid = !showGrid;
		ImGui::Text("Lights");
		if (ImGui::Button("Switch light"))
		{
			lightIsEnabled = !lightIsEnabled;
			light.SetIntencity(lightIsEnabled ? 1.f : 0.f);
		}
		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
		Time::Calculate();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void InitializeDependenciesAndWindow(GLFWwindow** window)
{
	if (!glfwInit())
		cout << "Glfw Init Error!";
	*window = glfwCreateWindow(width, height, "Xenon Slide", NULL, NULL);
	glfwMakeContextCurrent(*window);
	Window::window = *window;
	if (glewInit() != GLEW_OK)
		cout << "Glew Init Error!";
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(*window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}



