#pragma comment(lib, "GlU32.Lib")

#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>

#include "Mesh.h"
#include "LightSource.h"
#include "Mouse.h"
#include "Window.h"
#include "Physics.h"
#include "Selector.h"
#include "Grid.h"
#include "Primitives.h"
#include "Settings.h"
#include "SelectionPolygon.h"
#include "MeshDrag.h"
#include "GuiDrawer.h"
#include "timeManager.h"

const unsigned int width = 860;
const unsigned int height = 660;

using std::cout;
using std::endl;

using namespace settings;
using namespace MeshDrag;

std::vector<Vertex> vertices;
std::vector<GLuint> indices;
std::vector<int> multidrawVertsCount;

bool mouseIsOverMeshGui = false;
bool lightIsEnabled = false;
bool showGrid = true;
bool dragMode = false;

void InitializeDependenciesAndWindow(GLFWwindow** window);
Object* InitializeDefaultEnviroment();

int main()
{
	GLFWwindow* window;
	InitializeDependenciesAndWindow(&window);
	Object* object = InitializeDefaultEnviroment();

	glfwSetWindowUserPointer(window, &dragMode);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
			bool* dragMode = (bool*)glfwGetWindowUserPointer(window);
			if (key == GLFW_KEY_G && action == GLFW_PRESS)
			{
				*dragMode = !*dragMode;
			}
			else if (key == GLFW_KEY_E && action == GLFW_PRESS)
			{
				auto& obj = Selector::lastSelection->selectedObject;
				if (obj)
					obj->ExtrudeSelection();
			}
			else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
			{
				auto& obj = Selector::lastSelection->selectedObject;
				if (obj)
					obj->RemoveSelectedPolygon();
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
		
		if (SelectionPolygon::GetInstance().polygon && Selector::selectionMode == SelectionMode::Face)
			SelectionPolygon::GetInstance().UpdateVertices();
		else
			SelectionPolygon::GetInstance().polygonObject->mesh->ClearVerticesAndIndices();

		// Select polygons with a button click
		if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
		{
			glm::vec3 lastRayStart = glm::vec3(Camera::main->position.x, Camera::main->position.y, Camera::main->position.z);
			glm::vec3 lastRayEnd = Physics::CastRayFromScreenPoint();
			SelectionPolygon::GetInstance().polygon = Selector::SelectPolygonWithRay(lastRayStart, lastRayEnd);
		}

#pragma region RENDERING
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		if (showGrid) Grid::Render();
		glClear(GL_DEPTH_BUFFER_BIT);

		// Filled polygons
		SelectionPolygon::GetInstance().SetDefaultColor();
		for (auto obj : Object::objectsList)
		{
			obj->SetColor(Selector::selectionMode == SelectionMode::Object && Selector::lastSelection->selectedObject == obj ? polygonSelectorColor : defaultMeshColor);
			obj->Render();
		}
		SelectionPolygon::GetInstance().Render();

		if (lightIsEnabled)
			LightSource::RenderLights();

		// Wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPolygonOffset(-1.5f, -1.5f);
		SelectionPolygon::GetInstance().SetColor(glm::vec3(1.f));
		SelectionPolygon::GetInstance().Render();

		glPolygonOffset(-0.4f, -0.4f);
		for (auto obj : Object::objectsList)
		{
			obj->SetColor(Selector::selectionMode == SelectionMode::Object && Selector::lastSelection->selectedObject == obj ? glm::vec3(1.f) : glm::vec3(0.f));
			obj->Render();
		}
		SelectionPolygon::GetInstance().Render();

#pragma endregion
		if (dragMode)
			PerformDragWithMouse(Selector::lastSelection->selectedObject);

		Mouse::SetLastCoords();
		Mouse::SetLastRayFromMousePoint();

		GuiDrawer::DrawGui();

		glfwSwapBuffers(window);
		glfwPollEvents();
		Time::Calculate();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

/// <summary>
/// Initializes camera, grid and default cube
/// </summary>
/// <returns> Pointer to the default cube object </returns>
Object* InitializeDefaultEnviroment()
{
	Camera::main = new Camera(glm::vec3(0.0f, 1.0f, 1.0f));
	Grid::Init();

	SelectionPolygon::GetInstance().Initialize();

	Texture* texture = new Texture("./textures/pixel.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	Primitives::SetCubeVertices(&vertices, &indices, &multidrawVertsCount);
	Object* object = new Object(new Mesh(vertices, indices, nullptr, multidrawVertsCount), new Shader("./src/shaders/unlit.shader"));
	object->shader->SetUniform3f("color", glm::vec3(1.f, 1.f, 1.f));
	LightSource *light = new LightSource(glm::vec3(1.f, 1.f, 1.0f), 0, new Shader("./src/shaders/unlit.shader", ShaderType::Unlit));
	return object;
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
	GuiDrawer::InititlizeGui(*window);
}



