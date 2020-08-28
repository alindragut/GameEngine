#pragma once

#include <Core/Engine.h>

class BaseGameObject;

class ProjectileSpawner
{
public:

	ProjectileSpawner() {};
	~ProjectileSpawner() {};

	static BaseGameObject* SpawnArrow(BaseGameObject* creatorObject, glm::vec3 spawnPos, glm::vec3 dir, float speed, float maxTravelDistance);
	static BaseGameObject* SpawnMagicSpell(BaseGameObject* creatorObject, glm::vec3 spawnPos, glm::vec3 dir, float speed, float maxTravelDistance);
};