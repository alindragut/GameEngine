#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>

class AnimObjectGameComponent : public BaseComponent
{
public:
	GAME_COMPONENT(AnimObjectGameComponent)

	AnimObjectGameComponent();
	~AnimObjectGameComponent() {};

	void update(float deltaTimeSeconds) override;

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnKeyPress(int key, int mods) override;

private:
	int which;
	float speed;
};