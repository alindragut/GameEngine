#include "GameEngine.h"
#include "EngineManager.h"
#include <GameEngine/ImGUI/imgui.h>
#include <GameEngine/ImGUI/imgui_impl_opengl3.h>
#include <GameEngine/ImGUI/imgui_impl_glfw.h>
#include "btBulletDynamicsCommon.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/Utils/DebugDraw.h>
#include <GameEngine/Utils/ShaderCache.h>
#include <Component\CameraInput.h>

GameEngine::GameEngine() {
}

GameEngine::~GameEngine() {
	int size = objects.size();
	for (auto it = objects.begin(); it != objects.end();) {
		delete(*it);
		it = objects.erase(it);
	}
	delete physics;
	delete ImGui;
	delete sceneRenderer;
}

void GameEngine::Init() {
	EngineManager& em = EngineManager::GetInstance();
	em.SetGameEngine(this);

	ComponentFactory& factory = ComponentFactory::GetInstance();

	physics = new PhysicsEngine();
	physics->Init();

	ImGui = new ImGuiSetup();
	ImGui->Init();

	generator = new Generator(3, 30, 30, 3, 3, 4);
	generator->PlaceRooms();

	glm::ivec2 res = window->GetResolution();

	sceneRenderer = new ShadowMapSceneRenderer(&objects);
	sceneRenderer->Init(res.x, res.y, 256, 256, 1024, 1024);

	BaseGameObject* obj = factory.createObject(1);

	if (obj != nullptr) {
		objects.push_back(obj);
	}

	obj = factory.createObject(4);
	
	if (obj != nullptr) {
		objects.push_back(obj);
	}
	
	obj = factory.createObject(3);

	if (obj != nullptr) {
		obj->GetTransform()->SetPos(glm::vec3(-2, 0.5, 2));
		objects.push_back(obj);
	}

	obj = factory.createObject(3);

	if (obj != nullptr) {
		obj->GetTransform()->SetPos(glm::vec3(2, 0.5, -2));
		objects.push_back(obj);
	}

	obj = factory.createObject(3);

	if (obj != nullptr) {
		obj->GetTransform()->SetPos(glm::vec3(-2, 0.5, -2));
		objects.push_back(obj);
	}
	
	obj = factory.createObject(6);

	if (obj != nullptr) {
		obj->GetTransform()->SetScale(glm::vec3(20, 0.01, 20));
		obj->GetTransform()->SetPos(glm::vec3(0, -0.01, 0));
		objects.push_back(obj);
	}
	
	/*obj = factory.createObject(5);

	if (obj != nullptr) {
		obj->GetTransform()->SetPos(glm::vec3(-5, 0.5, -5));
		obj->GetTransform()->SetScale(glm::vec3(0.01));
		DefaultRenderer* dr = static_cast<DefaultRenderer*>(obj->GetComponent("DefaultRenderer"));
		dr->SetMesh("Source/GameEngine/Models", "SM_Bld_Rockwall_Stairs_01.fbx", "bridge", true, true);
		dr->SetShader("TextureMesh");
		objects.push_back(obj);
	}*/

	generator->Init();
}

void GameEngine::FrameStart() {
	ClearScreen();
	ImGui->NewFrame();
}

void GameEngine::Update(float deltaTimeSeconds) {
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("%.1f fps", 1.0f / deltaTimeSeconds);               // Display some text (you can use a format strings too)
	}

	physics->Step(deltaTimeSeconds);

	for (auto it = objects.begin(); it != objects.end();) {
		BaseGameObject* obj = *it;

		if (obj->ShouldDelete()) {
			toDelete.push_back(obj);
			it = objects.erase(it);
			
		}
		else {
			obj->update(deltaTimeSeconds);
			++it;
		}
	}

	while (!toDelete.empty()) {
		delete toDelete.front();
		toDelete.pop_front();
	}
	sceneRenderer->renderScene();
}

void GameEngine::FrameEnd() {
	DrawCoordinatSystem();
	ImGui->EndFrame();
}


void GameEngine::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseBtnPress(mouseX, mouseY, button, mods);
		}
		obj++;
	}
}

void GameEngine::OnInputUpdate(float deltaTime, int mods) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnInputUpdate(deltaTime, mods);
		}
		obj++;
	}
}
void GameEngine::OnKeyPress(int key, int mods) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnKeyPress(key, mods);
		}
		obj++;
	}
	if (key == GLFW_KEY_SPACE) {
		static_cast<CameraInput*>(GetCameraInput())->SetCameraLock(true);
	}
	if (key == GLFW_KEY_L) {
		static_cast<CameraInput*>(GetCameraInput())->SetCameraLock(false);
	}
}
void GameEngine::OnKeyRelease(int key, int mods) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnKeyRelease(key, mods);
		}
		obj++;
	}
}
void GameEngine::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseMove(mouseX, mouseY, deltaX, deltaY);
		}
		obj++;
	}
}
void GameEngine::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseBtnRelease(mouseX, mouseY, button, mods);
		}
		obj++;
	}
}
void GameEngine::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnMouseScroll(mouseX, mouseY, offsetX, offsetY);
		}
		obj++;
	}
}
void GameEngine::OnWindowResize(int width, int height) {
	for (auto obj = objects.begin(); obj != objects.end();) {
		auto components = (*obj)->GetComponents();
		for (const auto &it : (*components)) {
			it.second->OnWindowResize(width, height);
		}
		obj++;
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