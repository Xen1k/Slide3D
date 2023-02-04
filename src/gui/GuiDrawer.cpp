#include "Object.h"
#include "Window.h"
#include "Primitives.h"
#include "SelectionPolygon.h"
#include "Selector.h"
#include "LightSource.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GuiDrawer.h"


extern bool mouseIsOverMeshGui;
extern bool lightIsEnabled;
extern bool showGrid;
extern bool dragMode;
extern vector<Vertex> vertices;
extern vector<GLuint> indices;
extern vector<int> multidrawVertsCount;


void GuiDrawer::InititlizeGui(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void GuiDrawer::DrawGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Controls", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2(Window::GetWidth(), 90));
	ImGui::SetWindowPos(ImVec2(0, Window::GetHeight() - ImGui::GetWindowSize().y));
	ImGui::Text("E - Extrude");
	ImGui::Text("Delete - Remove selected polygon");
	ImGui::Text("G - Switch transformation (move) mode");
	ImGui::End();

	ImGui::Begin("Mesh/Texture/Light");
	mouseIsOverMeshGui = ImGui::IsWindowHovered();
	ImGui::Text("Mesh");
	if (ImGui::Button("Cube"))
	{
		Primitives::SetCubeVertices(&vertices, &indices, &multidrawVertsCount);
		SelectionPolygon::GetInstance().polygon = nullptr;
		Selector::lastSelection->Clear();
		Object* object = Object::objectsList[0];
		object->mesh->multidrawVertsCount = multidrawVertsCount;
		object->mesh->SetVerticesAndIndices(vertices, indices);
		object->CalculateFlatNormals();
	}
	ImGui::Text("Normals");
	if (ImGui::Button("Recalculate Flat"))
		for (auto obj : Object::objectsList)
			obj->CalculateFlatNormals();
	ImGui::Text("View");
	/*if (ImGui::Button("Switch Texture"))
	{
		Object::CallForEachObject([&texture](Object* object) {
			object->mesh->texture = object->mesh->texture ? nullptr : texture;
		});
	}*/
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
		LightSource::lightsList[0]->SetIntencity(lightIsEnabled ? 1.f : 0.f);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}