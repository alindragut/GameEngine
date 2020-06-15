#include "NPCSpawner.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/IncludeList.h>


BaseGameObject* NPCSpawner::SpawnNPC(BaseGameObject* creatorObject, glm::vec3 spawnPos) {
	BaseGameObject* obj = ComponentFactory::GetInstance().createObject(9);

	if (obj == nullptr) {
		return obj;
	}

	NPCMovementComponent* npc = static_cast<NPCMovementComponent*>(obj->GetComponent("NPCMovementComponent"));
	npc->SetSpawnPos(spawnPos);
	npc->SetTarget(creatorObject);
	npc->Init();

	EngineManager::GetInstance().GetGameEngine()->AddObject(obj);

	return obj;
}