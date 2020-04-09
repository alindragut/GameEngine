#pragma once

#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/Renderers/AnimationRenderer.h>
#include "AnimObjectGameComponent.h"
#include "AnimMovementGameComponent.h"

class AnimObject : public BaseGameObject
{
public:
	AnimObject(int objectId);
	~AnimObject();
	void update(float deltaTimeSeconds);

private:
	int which;
};