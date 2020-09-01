#include "NPCSpawner.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/IncludeList.h>


BaseGameObject* NPCSpawner::SpawnNPC(BaseGameObject* creatorObject, glm::vec3 spawnPos, int type) {
	
	BaseGameObject* obj;
	switch (type) {
	case 1:
		// LF a third model .dae to match the rest
		break;
	{
		 obj = ComponentFactory::GetInstance().createObject(13);

		if (obj == nullptr) {
			return obj;
		}
		SkeletonComponent* skeleton = static_cast<SkeletonComponent*>(obj->GetComponent("SkeletonComponent"));
		skeleton->SetSpawnPos(spawnPos);
		skeleton->SetTarget(creatorObject);
		skeleton->Init();
		break;
	}
	case 2:
	{
		obj = ComponentFactory::GetInstance().createObject(14);

		if (obj == nullptr) {
			return obj;
		}
		BruiserComponent* bruiser = static_cast<BruiserComponent*>(obj->GetComponent("BruiserComponent"));
		bruiser->SetSpawnPos(spawnPos);
		bruiser->SetTarget(creatorObject);
		bruiser->Init();
		break;
	}
	case 3:
	{
		obj = ComponentFactory::GetInstance().createObject(15);

		if (obj == nullptr) {
			return obj;
		}
		MageComponent* mage = static_cast<MageComponent*>(obj->GetComponent("MageComponent"));
		mage->SetSpawnPos(spawnPos);
		mage->SetTarget(creatorObject);
		mage->Init();
		break;
	}
	default:
		return obj;
	}

	EngineManager::GetInstance().GetGameEngine()->AddObject(obj);

	return obj;
}