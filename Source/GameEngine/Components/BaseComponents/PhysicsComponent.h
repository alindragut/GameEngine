#pragma once

#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/BaseComponents/Component.h>

class PhysicsComponent : public BaseComponent
{
public:
	PhysicsComponent() {}
	virtual void ComputeCollisions() {}
};