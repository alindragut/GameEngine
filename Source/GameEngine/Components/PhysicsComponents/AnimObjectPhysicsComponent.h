#pragma once

#include <GameEngine/Components/BaseComponents/PhysicsComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>

class AnimObjectPhysicsComponent : public PhysicsComponent
{
public:
	GAME_COMPONENT(AnimObjectPhysicsComponent)

	AnimObjectPhysicsComponent();
	~AnimObjectPhysicsComponent() {};

	void update(float deltaTimeSeconds) override;
	void ComputeCollisions() {};

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

private:
	int which;
};