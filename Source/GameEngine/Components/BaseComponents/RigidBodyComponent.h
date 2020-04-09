#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>

class btRigidBody;

class RigidBodyComponent : public BaseComponent
{
public:
	GAME_COMPONENT(RigidBodyComponent)

	RigidBodyComponent() {}
	~RigidBodyComponent() {}

	virtual void Init() override;

	virtual void update(float deltaTimeSeconds) {}

private:
	btRigidBody* body;
};