#pragma once

#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/BaseComponents/ComponentFactory.h>
#include <Component/SimpleScene.h>
#include <memory>

using namespace std;

class GameEngine : public SimpleScene
{
public:
	GameEngine();
	~GameEngine();

	void Init() override;

	Camera* GetCamera() { return GetSceneCamera(); }

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

	std::vector<std::unique_ptr<BaseGameObject>> objects;
};