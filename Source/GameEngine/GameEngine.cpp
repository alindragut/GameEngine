#include "GameEngine.h"
#include "EngineManager.h"
#include <GameEngine/ImGUI/imgui.h>
#include <GameEngine/ImGUI/imgui_impl_opengl3.h>
#include <GameEngine/ImGUI/imgui_impl_glfw.h>



GameEngine::GameEngine() {
}

GameEngine::~GameEngine() {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		objects[i].reset();
	}
}

void GameEngine::Init() {
	EngineManager& em = EngineManager::GetInstance();
	em.SetGameEngine(this);
	ComponentFactory& f = ComponentFactory::GetInstance();
	objects.push_back(f.createObject(1));
	objects.push_back(f.createObject(2));
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void GameEngine::FrameStart() {
	ClearScreen();
}

void GameEngine::Update(float deltaTimeSeconds) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		
		
	}
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		objects[i]->update(deltaTimeSeconds);
	}
	ImGui::End();
}

void GameEngine::FrameEnd() {
	DrawCoordinatSystem();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GameEngine::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseBtnPress(mouseX, mouseY, button, mods);
		}
	}
}

void GameEngine::OnInputUpdate(float deltaTime, int mods) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnInputUpdate(deltaTime, mods);
		}
	}
}
void GameEngine::OnKeyPress(int key, int mods) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnKeyPress(key, mods);
		}
	}
}
void GameEngine::OnKeyRelease(int key, int mods) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnKeyRelease(key, mods);
		}
	}
}
void GameEngine::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseMove(mouseX, mouseY, deltaX, deltaY);
		}
	}
}
void GameEngine::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseBtnRelease(mouseX, mouseY, button, mods);
		}
	}
}
void GameEngine::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseScroll(mouseX, mouseY, offsetX, offsetY);
		}
	}
}
void GameEngine::OnWindowResize(int width, int height) {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		auto components = objects[i]->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnWindowResize(width, height);
		}
	}
}