#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/Renderers/PointShadowRenderer.h>

class ArrowComponent : public BaseComponent
{
public:
	GAME_COMPONENT(ArrowComponent)

	ArrowComponent();
	~ArrowComponent() {};

	void update(float deltaTimeSeconds) override;
	void Init() override;

	void SetCreatorObject(BaseGameObject* creatorObject) { this->creatorObject = creatorObject; }
	void SetSpawnPos(glm::vec3 spawnPos) { this->spawnPos = spawnPos; }
	void SetDir(glm::vec3 dir) { this->dir = dir; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetMaxTravelDistance(float maxTravelDistance) { this->maxTravelDistance = maxTravelDistance; }

private:
	void UpdateModel();

	BaseGameObject* creatorObject;
	glm::vec3 spawnPos;
	glm::vec3 currentPos;
	glm::vec3 dir;
	float speed;
	float maxTravelDistance;
};