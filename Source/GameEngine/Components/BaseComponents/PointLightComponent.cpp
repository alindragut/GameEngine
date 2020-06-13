#include "PointLightComponent.h"

PointLightComponent::PointLightComponent() {
	worldPosition = glm::vec3(0);
}

void PointLightComponent::update(float deltaTimeSeconds) {
	SetWorldPosition(object->GetTransform()->GetPos());
}
