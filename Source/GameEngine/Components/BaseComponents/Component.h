#pragma once
#include "BaseGameObject.h"

#define GAME_COMPONENT(component_name) \
public: \
	std::string GetName () { return #component_name; }

class BaseComponent
{
public:
	virtual ~BaseComponent() {};
	virtual void update(float deltaTimeSeconds) = 0;

	virtual void OnInputUpdate(float deltaTime, int mods) {}
	virtual void OnKeyPress(int key, int mods) {}
	virtual void OnKeyRelease(int key, int mods) {}
	virtual void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
	virtual void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
	virtual void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
	virtual void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
	virtual void OnWindowResize(int width, int height) {}

	virtual std::string GetName() = 0;

	BaseGameObject* GetGameObject() { return this->object; }
	void SetGameObject(BaseGameObject* object) { this->object = object; }

protected:
	BaseGameObject* object;
};