#include "GameEngine.h"
#include "EngineManager.h"
#include <GameEngine/ImGUI/imgui.h>
#include <GameEngine/ImGUI/imgui_impl_opengl3.h>
#include <GameEngine/ImGUI/imgui_impl_glfw.h>
#include "btBulletDynamicsCommon.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/Utils/DebugDraw.h>

GameEngine::GameEngine() {
}

GameEngine::~GameEngine() {
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		delete objects[i];
	}
}

void GameEngine::Init() {
	EngineManager& em = EngineManager::GetInstance();
	em.SetGameEngine(this);

	ComponentFactory& factory = ComponentFactory::GetInstance();

	/*BaseGameObject* obj = factory.createObject(1);

	if (obj != nullptr) {
		objects.push_back(obj);
	}*/

	BaseGameObject* obj = factory.createObject(2);

	if (obj != nullptr) {
		objects.push_back(obj);
	}

	physics = new PhysicsEngine();
	physics->Init();

	int size = objects.size();
	for (int i = 0; i < size; i++) {
		objects[i]->InitComponents();
	}

	glm::ivec2 res = window->GetResolution();

	sceneRenderer = new ShadowMapSceneRenderer(&objects);
	sceneRenderer->Init(res.x, res.y, res.x, res.y);

	ImGui = new ImGuiSetup();
	ImGui->Init();

	//DebugDraw::DrawLine(glm::vec3(0), glm::vec3(2, 2, 2));
}

void GameEngine::FrameStart() {
	ClearScreen();
	ImGui->NewFrame();
}

void GameEngine::Update(float deltaTimeSeconds) {
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	}

	physics->Step(deltaTimeSeconds);

	int size = objects.size();
	for (int i = 0; i < size; i++) {
		objects[i]->update(deltaTimeSeconds);
	}
	sceneRenderer->renderScene();
}

void GameEngine::FrameEnd() {
	DrawCoordinatSystem();
	ImGui->EndFrame();
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

void ImGuiSetup::Init() {
	WindowObject* window = EngineManager::GetInstance().GetGameEngine()->GetWindow();

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

void ImGuiSetup::NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiSetup::EndFrame() {
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}