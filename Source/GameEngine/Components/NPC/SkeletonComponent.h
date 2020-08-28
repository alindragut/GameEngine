#pragma once

#include <GameEngine/Components/BaseComponents/BaseComponent.h>
#include <GameEngine/Components/BaseComponents/BaseGameObject.h>
#include <GameEngine/Components/Renderers/AnimationRenderer.h>
#include <GameEngine/Utils/AStarAlgorithm.h>
#include <stack>

class SkeletonComponent : public BaseComponent
{
public:
	GAME_COMPONENT(SkeletonComponent)

	SkeletonComponent();
	~SkeletonComponent() {};

	void Init() override;
	void update(float deltaTimeSeconds) override;

	void SetTarget(BaseGameObject* target) { this->target = target; };
	void SetSpawnPos(glm::vec3 spawnPos) { this->spawnPos = spawnPos; }
	void SetPathfindCooldown(float cooldown) { this->pathfindCooldown = cooldown; }

private:
	int state;
	// idle - 0
	// walking - 1
	// attack - 2

	glm::vec3 dir;
	glm::vec3 currentDest;
	glm::vec3 spawnPos;
	glm::vec3 targetPos;

	BaseGameObject* target;

	float pathfindCooldown;
	float timer;
	float speed;
	float crtAnimDuration;
	float crtAnimTimer;

	std::stack<std::pair<float, float>> crtPath;
	bool animation;
};
