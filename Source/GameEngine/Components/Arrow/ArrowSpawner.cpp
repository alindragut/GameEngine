#include "ArrowSpawner.h"
#include <GameEngine/Utils/ComponentFactory.h>
#include <GameEngine/IncludeList.h>


BaseGameObject* ArrowSpawner::SpawnArrow(BaseGameObject* creatorObject, glm::vec3 spawnPos, glm::vec3 dir, float speed, float maxTravelDistance) {
	BaseGameObject* obj = ComponentFactory::GetInstance().createObject(7);

	if (obj == nullptr) {
		return obj;
	}

	ArrowComponent* ac = static_cast<ArrowComponent*>(obj->GetComponent("ArrowComponent"));
	ac->SetCreatorObject(creatorObject);
	ac->SetSpawnPos(spawnPos);
	ac->SetDir(dir);
	ac->SetSpeed(speed);
	ac->SetMaxTravelDistance(maxTravelDistance);
	ac->Init();

	PointShadowRenderer* psr = static_cast<PointShadowRenderer*>(obj->GetComponent("PointShadowRenderer"));
	psr->SetMesh("arrow");

	EngineManager::GetInstance().GetGameEngine()->AddObject(obj);

	return obj;
}