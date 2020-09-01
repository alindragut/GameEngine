#pragma once

#include <GameEngine/SceneRenderers/ShadowMapSceneRenderer.h>
#include <GameEngine/Physics/PhysicsEngine.h>
#include <Component/SimpleScene.h>
#include <GameEngine/MapGenerator/Generator.h>
#include <memory>

using namespace std;

class btDiscreteDynamicsWorld;

class ImGuiSetup {
public:
	ImGuiSetup() {}
	~ImGuiSetup() {}

	void Init();

	void NewFrame();

	void EndFrame();
};

class GameEngine : public SimpleScene
{
public:
	GameEngine();
	~GameEngine();

	void Init() override;

	Camera* GetCamera() { return GetSceneCamera(); }

	std::list<BaseGameObject*> GetObjects() { return objects; }

	void AddObject(BaseGameObject* obj) { objects.push_back(obj); }

	WindowObject* GetWindow() { return window; }

	PhysicsEngine* GetPhysicsEngine() { return physics; }

	ShadowMapSceneRenderer* GetSceneRenderer() { return sceneRenderer; }

private:
	

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	bool showTutorial;
	BaseGameObject* test;
	ShadowMapSceneRenderer* sceneRenderer;
	PhysicsEngine* physics;
	ImGuiSetup* ImGui;
	Generator* generator;
	std::list<BaseGameObject*> objects;
	std::list<BaseGameObject*> toDelete;
};