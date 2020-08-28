#pragma once

#include <Core/Engine.h>

class BaseGameObject;

class NPCSpawner
{
public:

	NPCSpawner() {};
	~NPCSpawner() {};

	static BaseGameObject* SpawnNPC(BaseGameObject* creatorObject, glm::vec3 spawnPos, int type);
};