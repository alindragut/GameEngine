#pragma once

#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/PhysicsComponents/GameObjectPhysicsComponent.h>
#include <GameEngine/Components/Renderers/DefaultRenderer.h>

class GameObject : public BaseGameObject
{
public:
	GameObject(int objectId);
	~GameObject();
	void update(float deltaTimeSeconds);
};