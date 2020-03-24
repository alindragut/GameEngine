#pragma once

#include <GameEngine/Components/BaseComponents/PhysicsComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>

class GameObjectPhysicsComponent : public PhysicsComponent
{
public:
	GAME_COMPONENT(GameObjectPhysicsComponent)

	GameObjectPhysicsComponent();
	~GameObjectPhysicsComponent() {};

	void update(float deltaTimeSeconds) override;
	void ComputeCollisions() {};

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

private:
	const float speed = 1.0f;
	glm::vec3 dir;
};