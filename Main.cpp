#pragma comment(lib, "GlU32.Lib")

#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>
#include <stb/stb_image.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>

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
#include "MathUtils.h"
#include "Settings.h"
#include "SelectionPolygon.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "timeManager.h"

const unsigned int width = 860;
const unsigned int height = 660;

using std::cout;
using std::endl;
using std::vector;

using namespace settings;

vector<Vertex> vertices;
vector<GLuint> indices;
vector<int> multidrawVertsCount;

void InitializeDependenciesAndWindow(GLFWwindow** window);
void PrintVector(glm::vec3 vec) { cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << endl; }

int main()
{
	GLFWwindow* window;
	InitializeDependenciesAndWindow(&window);
	Camera::main = new Camera(glm::vec3(0.0f, 1.0f, 1.0f));
	Grid::Init();

	SelectionPolygon* selectionPolygon = new SelectionPolygon();

	Texture* texture = new Texture("./textures/pixel.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	Primitives::SetCubeVertices(&vertices, &indices, &multidrawVertsCount);
	Object *object = new Object(new Mesh(vertices, indices, nullptr, multidrawVertsCount), new Shader("./src/shaders/lit.shader"));
	object->shader->SetUniform3f("color", glm::vec3(1.f, 1.f, 1.f));

	LightSource light(glm::vec3(1.f, 1.f, 1.0f), 0, new Shader("./src/shaders/unlit.shader", ShaderType::Unlit));

	bool mouseIsOverMeshGui = false;


	bool lightIsEnabled = true;

	bool showWireframe = true;
	bool showGrid = true;


	bool dragMode = false;
	glfwSetWindowUserPointer(window, &dragMode);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
			bool* dragMode = (bool*)glfwGetWindowUserPointer(window);
			if (key == GLFW_KEY_G && action == GLFW_PRESS)
			{
				*dragMode = !*dragMode;
			}
			else if (key == GLFW_KEY_E && action == GLFW_PRESS)
			{
				// extrude
				auto obj = Selector::lastSelection->selectedObject;
				auto selectedPolygon = Selector::lastSelection->selectedPolygon;
				if (obj && selectedPolygon)
				{
					// Remove old polygon
					obj->mesh->RemovePolygon(selectedPolygon, false);

					vector<unsigned int> newIndices;
					// Indices
					for (int i = 0; i < selectedPolygon->verticesIndexNumbers.size(); i++)
					{
						obj->mesh->indices.push_back(obj->mesh->vertices.size() + i);
						newIndices.push_back(obj->mesh->vertices.size() + i);
					}
					for (int i = 0; i < selectedPolygon->vertices.size() - 1; i++)
					{
						obj->mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[i]);
						obj->mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[i + 1]);
						obj->mesh->indices.push_back(newIndices[i + 1]);
						obj->mesh->indices.push_back(newIndices[i]);
					}

					obj->mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[selectedPolygon->vertices.size() - 1]);
					obj->mesh->indices.push_back(selectedPolygon->verticesIndexNumbers[0]);
					obj->mesh->indices.push_back(newIndices[0]);
					obj->mesh->indices.push_back(newIndices[selectedPolygon->vertices.size() - 1]);

					// Vertices
					vector<Vertex> newVertices;
					for (auto vertex : selectedPolygon->vertices)
					{
						newVertices.push_back(*vertex);
						newVertices[newVertices.size() - 1].position += Math::DotV3(selectedPolygon->Forward(), 1.5f);
					}
					obj->mesh->vertices.insert(obj->mesh->vertices.end(), newVertices.begin(), newVertices.end());

					for(int i = 0; i < selectedPolygon->vertices.size() + 1 /* Sides + new front face */; i++)
						obj->mesh->multidrawVertsCount.push_back(selectedPolygon->verticesIndexNumbers.size());

					obj->mesh->GenerateMultidrawStartIndices();
					obj->GeneratePolygonsList();
					obj->mesh->SetVAO();

					
				}
			}
			else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
			{
				// remove polygon
				auto obj = Selector::lastSelection->selectedObject;
				auto selectedPolygon = Selector::lastSelection->selectedPolygon;
				if (obj && selectedPolygon)
				{
					obj->mesh->RemovePolygon(selectedPolygon);
					Selector::lastSelection->Clear();
					// remove selection polygon
				}
			}
		}
	);

	while (!glfwWindowShouldClose(window))
	{
		Time::SetNow();
		Window::ResizeViewport();
		glClearColor(60.f / 255, 60.f / 255, 60.f / 255, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLineWidth(1.5f / glm::distance(object->GetPosition(), Camera::main->position));

		Camera::main->HandleInputs(window, mouseIsOverMeshGui);
		
		if (selectionPolygon->polygon && Selector::selectionMode == SelectionMode::Face)
			selectionPolygon->UpdateVertices();
		else
			selectionPolygon->polygonObject->mesh->ClearVerticesAndIndices();

		if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
		{
			glm::vec3 lastRayStart = glm::vec3(Camera::main->position.x, Camera::main->position.y, Camera::main->position.z);
			glm::vec3 lastRayEnd = Physics::CastRayFromScreenPoint();
			selectionPolygon->polygon = Selector::SelectPolygonWithRay(lastRayStart, lastRayEnd);
		}

#pragma region RENDERING
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		if (showGrid) Grid::Render();
		glClear(GL_DEPTH_BUFFER_BIT);

		// Filled polygons
		selectionPolygon->SetDefaultColor();
		for (auto obj : Object::objectsList)
		{
			obj->SetColor(Selector::selectionMode == SelectionMode::Object && Selector::lastSelection->selectedObject == obj ? polygonSelectorColor : defaultMeshColor);
			obj->Render();
		}
		selectionPolygon->Render();

		if (lightIsEnabled) light.Render(*Camera::main);

		// Wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPolygonOffset(-1.5f, -1.5f);
		selectionPolygon->SetColor(glm::vec3(1.f));
		selectionPolygon->Render();

		glPolygonOffset(-0.4f, -0.4f);
		if (showWireframe)
		{
			for (auto obj : Object::objectsList)
			{
				obj->SetColor(Selector::selectionMode == SelectionMode::Object && Selector::lastSelection->selectedObject == obj ? glm::vec3(1.f) : glm::vec3(0.f));
				obj->Render();
			}
		}
		selectionPolygon->Render();

#pragma endregion


		if (dragMode)
		{
			Object* obj = Selector::lastSelection->selectedObject;
			if (obj)
			{
				if (Selector::selectionMode == SelectionMode::Face)
				{
					for (int i = 0; i < Selector::lastSelection->GetNumOfSelections(); i++)
					{
						obj->mesh->vertices[Selector::lastSelection->selectedVerticesIndexNumbers[i]].position += Math::DotV3(Mouse::GetRaysFromMousePointDelta(),
							glm::distance(Camera::main->position, obj->GetPosition()));
					}
					if (selectionPolygon->polygon)
						selectionPolygon->UpdateVertices();
					obj->CalculateFlatNormals();
				}
				else
				{
					obj->Translate(Math::DotV3(Mouse::GetRaysFromMousePointDelta(), glm::distance(Camera::main->position, obj->GetPosition())));
				}
			}
		}

#pragma region GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Mesh/Texture/Light");
		mouseIsOverMeshGui = ImGui::IsWindowHovered();
		ImGui::Text("Mesh");
		if (ImGui::Button("Cube"))
		{
			Primitives::SetCubeVertices(&vertices, &indices, &multidrawVertsCount);
			selectionPolygon->polygon = nullptr;
			Selector::lastSelection->Clear();
			object->mesh->multidrawVertsCount = multidrawVertsCount;
			object->mesh->SetVerticesAndIndices(vertices, indices);
			object->CalculateFlatNormals();
		}
		ImGui::Text("Normals");
		if (ImGui::Button("Recalculate Flat"))
			for (auto obj : Object::objectsList)
				obj->CalculateFlatNormals();
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
		ImGui::Text("Selection Mode");
		if (ImGui::Button("Face"))
			Selector::selectionMode = SelectionMode::Face;
		if (ImGui::Button("Object"))
			Selector::selectionMode = SelectionMode::Object;
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
		Mouse::SetLastCoords();
		Mouse::SetLastRayFromMousePoint();
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
	glEnable(GL_POLYGON_OFFSET_LINE);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glPolygonOffset(-1, -1);
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(*window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}



