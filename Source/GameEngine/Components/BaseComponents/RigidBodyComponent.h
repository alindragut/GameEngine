#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>

class btRigidBody;

class RigidBodyComponent : public BaseComponent
{
public:
	GAME_COMPONENT(RigidBodyComponent)

	RigidBodyComponent() { this->walkable = false; }
	~RigidBodyComponent();

	void SetWalkable(bool walkable) { this->walkable = walkable; }

	virtual void Init() override;

	virtual void update(float deltaTimeSeconds) override;

private:
	btRigidBody* body;
	bool walkable;

	glm::vec3 prevPos;
	glm::vec3 prevScale;
};