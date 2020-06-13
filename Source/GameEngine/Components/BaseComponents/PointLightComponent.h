#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>

class PointLightComponent : public BaseComponent
{
public:
	GAME_COMPONENT(PointLightComponent)

	PointLightComponent();
	~PointLightComponent() {};

	void update(float deltaTimeSeconds) override;

	glm::vec3 GetWorldPosition() { return worldPosition; }
	void SetWorldPosition(glm::vec3 worldPosition) { this->worldPosition = worldPosition; }

private:
	glm::vec3 worldPosition;
};