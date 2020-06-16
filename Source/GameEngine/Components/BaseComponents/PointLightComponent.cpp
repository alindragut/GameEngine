#include "PointLightComponent.h"
#include <GameEngine/IncludeList.h>

PointLightComponent::PointLightComponent() {
	worldPosition = glm::vec3(0);
}

void PointLightComponent::update(float deltaTimeSeconds) {
	SetWorldPosition(object->GetTransform()->GetPos() + glm::vec3(0, 0.5, 0));
	EngineManager::GetInstance().GetGameEngine()->GetSceneRenderer()->SetLightPos(worldPosition);
}
