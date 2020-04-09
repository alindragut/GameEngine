#pragma once

#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include "GameObjectGameComponent.h"
#include <GameEngine/Components/Renderers/DefaultRenderer.h>
#include <GameEngine/Components/BaseComponents/RigidBodyComponent.h>

class GameObject : public BaseGameObject
{
public:
	GameObject(int objectId);
	~GameObject();
	void update(float deltaTimeSeconds);
};