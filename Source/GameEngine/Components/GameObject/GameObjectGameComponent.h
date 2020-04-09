#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>

class GameObjectGameComponent : public BaseComponent
{
public:
	GAME_COMPONENT(GameObjectGameComponent)

	GameObjectGameComponent();
	~GameObjectGameComponent() {};

	void update(float deltaTimeSeconds) override;

	void Init() override;

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

private:
	const float speed = 1.0f;
	glm::vec3 dir;
};