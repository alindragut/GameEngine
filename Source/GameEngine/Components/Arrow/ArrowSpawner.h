#pragma once

#include <Core/Engine.h>

class BaseGameObject;

class ArrowSpawner
{
public:

	ArrowSpawner() {};
	~ArrowSpawner() {};

	static BaseGameObject* SpawnArrow(BaseGameObject* creatorObject, glm::vec3 spawnPos, glm::vec3 dir, float speed, float maxTravelDistance);
};